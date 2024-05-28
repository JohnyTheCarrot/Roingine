const SCRIPT_NAME = "RuntimeScript";

function Init() {
    roingine.println("This script was added at runtime!");
}

input.onKeyDown(input.KEY_P, function() {
    roingine.println("P key pressed!");
});

function Update() {
    roingine.println("properties", current.properties);
}
