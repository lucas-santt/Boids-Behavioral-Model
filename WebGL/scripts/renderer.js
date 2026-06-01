var canvas, gl, globalDefaultShaderProgram, globalBoidShaderProgram;
var triangleVAO, CircleVAO;

var deltaTime = 0.0, lastFrame = 0.0, runTime = 0.0;
var isPaused = false, runOneFrame = false;
var fixedObjectsCount;

var GROUP_COLORS = [];

const keys = {
    ArrowUp:    false,
    ArrowDown:  false,
    ArrowRight: false,
    ArrowLeft:  false
}

window.onload = main;
window.onkeydown = callbackKeyDown;
window.onkeyup = callbackKeyUp;
window.onmousemove = callbackMouseMove;


function main() {
    /*
        Responsável por inicializar o programa e 
            começar o loop de frames
    */
    canvas = document.getElementById("meucanvas");
    gl = canvas.getContext("webgl2");
    if(!gl) console.log("WebGL 2 not supported in this browser");

    gl.enable(gl.BLEND);
    
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

    globalDefaultShaderProgram = createShaderProgram(gsVertexShaderSrc, gsFragmentShaderSrc);
    globalBoidShaderProgram    = createShaderProgram(gsBoidVertexShaderSrc, gsFragmentShaderSrc);

    addObstacles();
    fixedObjectsCount = SCENE_OBJECTS.length;

    for(let i=1; i<NUMBER_OF_GROUPS; i++) {
        GROUP_COLORS.push(randomColor());
    }

    for(const obj of SCENE_OBJECTS) {
        if(obj instanceof Boid) { obj.init(gl, globalBoidShaderProgram, random(0, NUMBER_OF_GROUPS)) }
        else { obj.init(gl, globalDefaultShaderProgram); }
    }

    triangleVAO = createVertexArray(TriangleObject.getVertices(), globalBoidShaderProgram);
    CircleVAO   = createVertexArray(CircleObject.getVertices(), globalDefaultShaderProgram);

    lastFrame = Date.now();
    requestAnimationFrame(frameUpdate);
}

// WebGL

function frameUpdate() {
    /*
        Responsable for running a frame

        Updates the canvas resolution (for resposiveness)
    */
    let currFrame = Date.now();
    deltaTime = (currFrame - lastFrame) / 1000.0;
    lastFrame = currFrame;
    runTime += deltaTime;

    updateCanvasResolution();

    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
    
    gl.clearColor(...BACKGROUND_COLOR);
    gl.clear(gl.COLOR_BUFFER_BIT);


    for(const obj of SCENE_OBJECTS) {
        vao = triangleVAO

        if(obj instanceof Boid) { if(!isPaused) obj.updatePosition(); } 
        else { vao = CircleVAO; }

        obj.draw(vao);
    }

    if(runOneFrame) { runOneFrame = false; isPaused = true }

    requestAnimationFrame(frameUpdate);
}

function createShader(gl, type, source) {
    /*
        Creates a specific type of shader
    */
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        console.error(gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }
    return shader;
}

function createShaderProgram(vertexSrc, fragmentSrc) {
    /*
        Creates a shader program with a vertex and fragment shader
    */
    const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexSrc);
    const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentSrc);

    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);

    return program;
}

function createVertexArray(vertices, shaderProgram) {
    /*
        Creates a Vertex Array Object that has a buffer
        for specific vertices.

        This function only works with vertices, so the VBO has
        a stride and an offset of 0
    */
    vao = gl.createVertexArray();
    gl.bindVertexArray(vao);

    const buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    const aPositionLoc = gl.getAttribLocation(shaderProgram, "aPosition");
    gl.enableVertexAttribArray(aPositionLoc);
    gl.vertexAttribPointer(aPositionLoc, 2, gl.FLOAT, false, 0, 0);

    gl.bindVertexArray(null);
    return vao;
}

// Game

function moveLeader() {
    /*
        Move the leader based on pressed inputs,
            the only that are true in the keys object
    */
    let dir = new Vector2(0.0, 0.0);

    if(keys.ArrowLeft)  { dir.x -= 1; }
    if(keys.ArrowRight) { dir.x += 1; }
    if(keys.ArrowUp)    { dir.y += 1; }
    if(keys.ArrowDown)  { dir.y -= 1; }

    if(dir.magnitude() == 0) { return; }
    SCENE_OBJECTS[0].move(dir.normalize()); // leader is always the first object
}

function addObstacles() {
    /* 
        Add random objects to the scene
    */
    for(let i=0; i < OBSTACLE_COUNT; i++) {
        let pos = new Vector2(random(-1, 1), random(-1, 1));
        SCENE_OBJECTS.push(new Obstacle(pos));
    }
}

function addBoid() {
    /*
        Add a Boid to the scene roister,
            it has random position, velocity and
            velocity direction
    */
    let pos = new Vector2(random(-1, 1), random(-1, 1));

    let velDir = new Vector2(random(-1, 1), random(-1, 1)).normalize();
    let vel = random(...RANDOM_VEL_RANGE);

    let group = Math.floor(random(0, NUMBER_OF_GROUPS));

    let newBoid = new Boid(pos, velDir.normalize(), vel, GROUP_COLORS[group], new Vector2(0.1, 0.2));
    newBoid.init(gl, globalBoidShaderProgram, group);

    SCENE_OBJECTS.push(newBoid);
}

function removeBoid() {
    /*
        Remove a boid from the scene,
            it needs the fixedObjectsCount to
            not remove obstacles or the leader
    */
    if(SCENE_OBJECTS.length == fixedObjectsCount) return;

    SCENE_OBJECTS.pop();
}

// Support Functions

function updateCanvasResolution() {
    /*
        Updates the canvas resolution according with its
        pixel ratio, it is necessary to keep the software
        responsive and to not blury it out
    */
    const dpr = window.DevicePixelRatio || 1;

    const cssWidth = window.innerWidth;
    const cssHeight = window.innerHeight;

    // Display size
    canvas.style.width = cssWidth + "px";
    canvas.style.height = cssHeight + "px";

    // Actual size in memory
    canvas.width = Math.round(cssWidth * dpr);
    canvas.height = Math.round(cssHeight * dpr);
}

function random(min, max) {
    /* Returns a random number between min and max */
    return Math.random() * (max - min) + min;
}

function randomColor() {
    /* Returns a random color */
    return [random(0, 1), random(0, 1), random(0, 1), 1.0]
}

// Callbacks

function callbackKeyDown(event) {
    /* 
        Checks wich key is pressed
            and executes its respective
            function
    */
    const keyName = event.key;

    // + and = is the same key
    if(keyName === '+' || keyName === '=') {
        addBoid();
    } else if (keyName === '-') {
        removeBoid();
    }

    if(keyName === 'p') {
        isPaused = !isPaused;
    }

    if(isPaused) {
        if(keyName === 's') {
            isPaused = false;
            runOneFrame = true;
        }
        return;
    }

    if(keys.hasOwnProperty(keyName)) {
        keys[keyName] = true;
        moveLeader();
    }
}

function callbackKeyUp(event) {
    /*
        Check if any movement key
            is unpressed and updates
            the keys object accordantly
    */
    const keyName = event.key;

    if(keys.hasOwnProperty(keyName)) {
        keys[keyName] = false;
        moveLeader();
    }
}

function callbackMouseMove(event) {
    const pixelX = event.clientX;
    const pixelY = event.clientY;

    let normalizedX = (pixelX / canvas.width) * 2.0 - 1.0;
    let normalizedY = 1.0 - (pixelY / canvas.height) * 2.0;

    SCENE_OBJECTS[1].move(normalizedX, normalizedY);
}

// Shaders

var gsVertexShaderSrc = `#version 300 es

in vec4 aPosition;

uniform vec2 uTranslation;
uniform vec2 uScale;
uniform vec2 uResolution;
uniform float uLookAtAngle;
uniform float uTime;

void main() {
    vec2 scaledPos = vec2(aPosition.x * uScale.x, aPosition.y * uScale.y);
    
    float cs = cos(uLookAtAngle);
    float sn = sin(uLookAtAngle);
    vec2 rotatedPos = vec2(
        scaledPos.x * cs - scaledPos.y * sn, 
        scaledPos.x * sn + scaledPos.y * cs
    );
    
    float aspectRatio = uResolution.x / uResolution.y;
    rotatedPos.x /= aspectRatio;

    vec2 pixelPos = rotatedPos + uTranslation;

    gl_Position = vec4(pixelPos, 0.0, 1.0);
}

`

var gsBoidVertexShaderSrc = `#version 300 es

in vec4 aPosition;

uniform vec2 uTranslation;
uniform vec2 uScale;
uniform vec2 uResolution;
uniform float uLookAtAngle;
uniform float uTime;

void main() {
    float flapSpeed  = 10.0;
    float flapAmp    = aPosition.x * 2.4;
    float flapOffset = aPosition.x * 3.0;

    float flap = 1.0 + (sin((uTime * flapSpeed) + flapOffset) * flapAmp);

    vec2 scaledPos = vec2(aPosition.x * uScale.x * flap, aPosition.y * uScale.y);
    
    float cs = cos(uLookAtAngle);
    float sn = sin(uLookAtAngle);
    vec2 rotatedPos = vec2(
        scaledPos.x * cs - scaledPos.y * sn, 
        scaledPos.x * sn + scaledPos.y * cs
    );
    
    float aspectRatio = uResolution.x / uResolution.y;
    rotatedPos.x /= aspectRatio;

    vec2 pixelPos = rotatedPos + uTranslation;

    gl_Position = vec4(pixelPos, 0.0, 1.0);
}

`

var gsFragmentShaderSrc = `#version 300 es

precision highp float;

uniform vec4 uColor;
out vec4 outColor;

void main() {
    outColor = uColor;
}

`