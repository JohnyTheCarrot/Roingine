const SCRIPT_NAME = "RuntimeScript";

function Init() {
    roingine.println("This script was added at runtime!");
}

// input.onKeyDown(input.KEY_P, function() {
//     roingine.println("P key pressed!");
// });

script.api = {
    testMethod: function (message) {
        roingine.println("from RuntimeScript", message);
        return "return value from RuntimeScript method";
    }
}
