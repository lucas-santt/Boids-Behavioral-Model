class Vector2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    magnitude() {
        /* Returns the magnitude (size) of the vector */
        return Math.sqrt( (this.x*this.x) + (this.y*this.y));
    }

    normalize() {
        /* Returns the vector normalized */
        let mag = this.magnitude();
        if(mag==0) return new Vector2(0.0, 0.0);
        
        return new Vector2(this.x / mag, this.y / mag);
    }

    static add(v1, v2) {
        /* Adds two vectors */
        return new Vector2(v1.x + v2.x, v1.y + v2.y);
    }

    static subtract(v1, v2) {
        /* Subtract vector v1 from vector v2 */
        return new Vector2(v1.x - v2.x, v1.y - v2.y);
    }

    static distance(v1, v2) {
        /* Return the distance between two points */
        let xDiff = v1.x - v2.x;
        let yDiff = v1.y - v2.y;

        let dis = Math.sqrt( (xDiff*xDiff) + (yDiff*yDiff) );
        return Math.abs(dis);
    }

    static mult(v, c) {
        /* Multiplies a vector v by a constant c */
        return new Vector2(v.x * c, v.y * c);
    }

    static divide(v, c) {
        /* Divides a vector v by a constant c */
        if(c == 0) { return new Vector2(0.0, 0.0) }
        return new Vector2(v.x / c, v.y / c);
    }

}

class SceneObject {
    constructor(position, scale, color) {
        /*
            SceneObject class represents every object in our program

            It is a abstract class, alone it has no value but when inherited
            will take care of the initialization and drawing of the object

            The only thing needed to be done by the children of this class
            is implement the _getAngle() function, wich is specific for each 
            type of object
        */
        this.pos = position;
        
        this._scale = scale;
        this._color = color;
    }

    init(gl, shaderProgram) {
        /*
            Initiates the object by storing
                specific configurations and uniforms location
        */
        this._gl = gl;
        this._shaderProgram = shaderProgram

        this._uTranslationLoc = gl.getUniformLocation(shaderProgram, "uTranslation");
        this._uScaleLoc       = gl.getUniformLocation(shaderProgram, "uScale");
        this._uResolutionLoc  = gl.getUniformLocation(shaderProgram, "uResolution");
        this._uColorLoc       = gl.getUniformLocation(shaderProgram, "uColor");
        this._uLookAtAngleLoc = gl.getUniformLocation(shaderProgram, "uLookAtAngle");
        this._uTimeLoc        = gl.getUniformLocation(shaderProgram, "uTime");
    }

    draw(vao, verticeCount) {
        /* 
            Draw the object

            Set uniforms values and bind the vao
        */
        const gl = this._gl;
        gl.useProgram(this._shaderProgram);

        gl.uniform2f(this._uTranslationLoc, this.pos.x, this.pos.y);
        gl.uniform2f(this._uScaleLoc, this._scale.x, this._scale.y);
        gl.uniform2f(this._uResolutionLoc, canvas.width, canvas.height);
        gl.uniform4f(this._uColorLoc, ...this._color);
        gl.uniform1f(this._uTimeLoc, runTime);

        let angle = this._getAngle();
        gl.uniform1f(this._uLookAtAngleLoc, angle);

        gl.bindVertexArray(vao);
        gl.drawArrays(gl.TRIANGLES, 0, verticeCount);
    }

    _getAngle() {
        /* Needs to be implemented by the children of this object */
        throw Error("Get Angle not implemented");
    }
}

class TriangleObject extends SceneObject {
    static getVertices() {
        /* Returns a unit triangle shape */
        return new Float32Array([
            0.0,   0.25,   // Upper vertex
           -0.25, -0.25,  // Lower Left
            0.25, -0.25,  // Lower Right
        ])
    }

    draw(vao) {
        /* Draw the object with only 3 vertices */
        super.draw(vao, 3);
    }
}

class CircleObject extends SceneObject {
    constructor(position, radius, color) {
        /* This class represents any circled object */
        super(position, new Vector2(radius, radius), color);

        this.radius = radius;
    }

    draw(vao) {
        /* 
            Draw the object, multiply the resolution by 3 because it has
                the triple amount of vertices of its resolution
        */ 
        super.draw(vao, CIRCLE_RESOLUTION * 3);
    }

    _getAngle() {
        /* Since it is a circle, the angle doesnt matter */
        return 0;
    }

    static getVertices() {
        /* Creates the vertices for a circle */
        const vertices = [];

        for(let i=0; i < CIRCLE_RESOLUTION; i++) {
            const theta = (i / CIRCLE_RESOLUTION) * Math.PI * 2;
            const nextTheta = ((i+1) / CIRCLE_RESOLUTION) * Math.PI * 2;

            vertices.push(0, 0);
            vertices.push(Math.cos(theta), Math.sin(theta));
            vertices.push(Math.cos(nextTheta), Math.sin(nextTheta));
        }

        return new Float32Array(vertices);
    }
}

class Boid extends TriangleObject {
    constructor(
        position, 
        velocityDir, 
        velocity = 0.5, 
        color = BOID_COLOR, 
        scale = new Vector2(0.2, 0.3), 
        visionRadius = DEFAULT_BOID_VISION_RANGE) {
        /*
            Represents a boid object

            It takes care of the movement algorithm of
                a boid, in wich it can calculate the grouping actions
                of it
        */
        super(position, scale, color);

        this.velDir = velocityDir;
        this.vel = velocity;

        this._visionRadius = visionRadius;
    }

    init(gl, shaderProgram, group = 0) {
        this.group = group;
        super.init(gl, shaderProgram);
    }

    updatePosition(groupMovement = true) {
        /* 
            Updates the movement of the boid based on time passed

            If groupMovement is true, it also calculates the steering behaviour
                of the object so it follow the steering rules
        */
        if(groupMovement) { this._updateVelocity(); }

        let currVel    = Vector2.mult(this.velDir, deltaTime * this.vel);
        let newPos     = Vector2.add(this.pos, currVel);
        
        if(newPos.x < -1.0) { newPos.x = -1.0; this.velDir.x = -this.velDir.x }
        if(newPos.y < -1.0) { newPos.y = -1.0; this.velDir.y = -this.velDir.y }
        if(newPos.x >= 1.0) { newPos.x =  1.0; this.velDir.x = -this.velDir.x }
        if(newPos.y >= 1.0) { newPos.y =  1.0; this.velDir.y = -this.velDir.y }
    
        this.pos = newPos;
    }

    _updateVelocity() {
        /* 
            Calculates the steering behaviour of the boid,
                including separation, alignment and cohesion

            Here, the velocity is changes based on its group movement
        */
        let sepDir = new Vector2(0.0, 0.0);
        let algDir = new Vector2(0.0, 0.0);
        let algVel = 0.0;
        let barycenter = new Vector2(0.0, 0.0);

        let boidCount = 0;

        for(const obj of SCENE_OBJECTS) {
            if(obj == this) continue;

            let dist = Vector2.distance(obj.pos, this.pos);
            if(dist > this._visionRadius) continue;

            // Separation
            let repulse = Vector2.subtract(this.pos, obj.pos);
            repulse = Vector2.divide(repulse, dist);
            sepDir = Vector2.add(sepDir, repulse);

            if(obj instanceof Boid && obj.group === this.group) {
                // Alignment
                algDir = Vector2.add(algDir, obj.velDir);
                algVel += obj.vel;
                // Cohesion
                barycenter = Vector2.add(barycenter, obj.pos);
                
                boidCount++;
            } else {
                sepDir = Vector2.mult(sepDir, OBSTACLE_SEPARATION_WEIGHT);
            }
        }
        sepDir = Vector2.mult(sepDir, SEPARATION_WEIGHT); // We do not normalize separation

        algDir = Vector2.divide(algDir, boidCount).normalize();
        algDir = Vector2.mult(algDir, ALIGNMENT_WEIGHT);
        algVel = (algVel + this.vel) / (boidCount + 1);
        algVel *= ALIGNMENT_WEIGHT;

        barycenter = Vector2.divide(barycenter, boidCount);
        let cohDir = Vector2.subtract(barycenter, this.pos).normalize();
        cohDir = Vector2.mult(cohDir, COHESION_WEIGHT);

        let resultDir = Vector2.add(sepDir, cohDir);
        resultDir = Vector2.add(resultDir, algDir);

        this.velDir = Vector2.add(this.velDir, Vector2.mult(resultDir, STEER_FORCE)).normalize();
        this.vel = algVel;
    }

    _getAngle() {
        /* Returns the angle in wich the boid is moving into */
        return Math.atan2(this.velDir.y, this.velDir.x) - (Math.PI / 2)
    }
}

class Leader extends Boid {
    constructor(position, scale, color = LEADER_COLOR) {
        /* 
            Represents the boid leader, in wich is responsible for
                calculating the movement based on a direction input
        */
        super(position, new Vector2(1.0, 1.0).normalize(), 0.5, color, scale);
    }

    move(direction) {
        /* 
            Move the boid based on an input
            
            We add the direction to the velocity because the leader
                has a semi-automatic movement, we need him to keep
                moving even with a lack of user inputs
        */
        this.velDir = Vector2.add(direction, this.velDir).normalize();
    }

    updatePosition() {
        /* 
            Updates the position of the leader,
                it doesnt calculates the steering
                behaviour since the leader only cares
                about the user input
        */
        super.updatePosition(false);
    }
}

class Obstacle extends CircleObject {
    constructor(position, radius = 0.02, color = OBSTACLE_COLOR) {
        /* Represents a Obstacles that needs to be avoided by the boids */
        super(position, radius, color);
    }
}

class Predator extends Obstacle {
    move(x, y) {
        this.pos = new Vector2(x, y);
    }
}