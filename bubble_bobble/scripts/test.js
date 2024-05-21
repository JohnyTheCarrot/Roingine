const SPEED = 200;

var transform;

function translate(x, y) {
    const transX = roingine.getDeltaTime() * x;
    const transY = roingine.getDeltaTime() * y;

    transform.translate(transX, transY);
}

input.onKeyHeld(input.KEY_W, function() {
    translate(0, SPEED);
});

input.onKeyHeld(input.KEY_S, function() {
    translate(0, -SPEED);
});

input.onKeyHeld(input.KEY_A, function() {
    translate(-SPEED, 0);
});

input.onKeyHeld(input.KEY_D, function() {
    translate(SPEED, 0);
});

function Init() {
    roingine.println("Test Initialized!");

    transform = gameObject.addComponent("Transform", 200, 100);
    gameObject.addComponent("Rect", 50, 50);
    gameObject.addComponent("RectRenderer");
}
