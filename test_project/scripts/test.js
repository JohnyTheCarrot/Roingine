const SCRIPT_NAME = "Test";

const SPEED = 200;

var transform;
var requireTest = require('require_test.js');
var runtimeScript;
var rectRenderer;

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
    var other = scene.getGameObject(0);
});

function Init(a, b, c) {
    roingine.println("Test Initialized!", a, b, c);

    transform = current.addComponent("Transform", 200, 100);
    var collider = current.addComponent("RectCollider", 50, 50);

    collider.onCollision(function(gameObject, hitX, hitY) {
        var otherTransform = gameObject.getComponent("Transform");
        otherTransform.translate(1, 0);
        transform.setLocalPosition(hitX, hitY);
    });

    scripts.addScript("scripts/runtime_script.js");

    runtimeScript = scripts.getScript("RuntimeScript");

    runtimeScript.setProperty("stringTest", "bingus");
    runtimeScript.setProperty("boolTest", true);
    var result = runtimeScript.callMethod("testMethod", "Hello world from test.js!");
    roingine.println(result);

    roingine.println(script.callCpp("cppCallTest", 42, 24));

    current.addComponent("Rect", 50, 50);
    rectRenderer = current.addComponent("RectRenderer");
    rectRenderer.setColor(0xFF0000);

    requireTest.hello();
    script.setEventListener("test_event", function (a, b, c){ roingine.println("Event caught!", a, b, c); });
    roingine.fireEvent("test_event", 1, 2, 3);
}

function OnDispose() {
    roingine.println("OnDispose");
}