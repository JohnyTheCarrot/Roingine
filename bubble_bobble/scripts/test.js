const SCRIPT_NAME = "Test";

const SPEED = 200;

var transform;
var scripts;
var requireTest = require('require_test.js');
var runtimeScript;

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

input.onKeyHeld(input.KEY_T, function() {
    var value = runtimeScript.getProperty("prop") || 0;
    runtimeScript.setProperty("prop", ++value);
});

function Init() {
    roingine.println("Test Initialized!");

    transform = current.addComponent("Transform", 200, 100);
    var collider = current.addComponent("RectCollider", 50, 50);

    collider.onCollision(function(gameObject, hitX, hitY) {
        var otherTransform = gameObject.getComponent("Transform");
        otherTransform.translate(1, 0);
        transform.setLocalPosition(hitX, hitY);
    });

    scripts = current.getComponent("Scripts");
    scripts.addScript("scripts/runtime_script.js");

    runtimeScript = scripts.getScript("RuntimeScript");

    runtimeScript.setProperty("stringTest", "bingus");
    runtimeScript.setProperty("boolTest", true);
    var result = runtimeScript.callMethod("testMethod", "Hello world from test.js!");
    roingine.println(result);

    current.addComponent("Rect", 50, 50);
    current.addComponent("RectRenderer");

    requireTest.hello();
}

function OnDispose() {
    roingine.println("OnDispose");
}
