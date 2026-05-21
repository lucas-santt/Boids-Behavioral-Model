// Generic Configurations

const BACKGROUND_COLOR = [0.9, 0.95, 1, 1.0];
const CIRCLE_RESOLUTION = 32;

const BOID_COLOR = [0.3, 0.6, 0.7, 1.0];
const LEADER_COLOR = [1.0, 0.6, 0.3, 1.0];

const RANDOM_VEL_RANGE = [0.1, 0.8];

// Steering Behavior

const DEFAULT_BOID_VISION_RANGE = 0.1;

const SEPARATION_WEIGHT = 0.5;
const ALIGNMENT_WEIGHT  = 1.0;
const COHESION_WEIGHT   = 1.0;

const STEER_FORCE = 0.1;

NUMBER_OF_GROUPS = 2;

// Obstacles

const OBSTACLE_COUNT = 10;
const OBSTACLE_COLOR = [1.0, 0.2, 0.4, 1.0]
const PREDATOR_COLOR = [1.0, 0.2, 0.4, 0.7]

const OBSTACLE_SEPARATION_WEIGHT = 3;

// Objects
// Leader MUST BE the first object
// Predator MUST BE the second one
const SCENE_OBJECTS = [
    new Leader(new Vector2(0, 0, 0)),
    new Predator(new Vector2(0, 0), 0.05, PREDATOR_COLOR)
]

GROUP_COLORS = [
    BOID_COLOR // I want this color
]