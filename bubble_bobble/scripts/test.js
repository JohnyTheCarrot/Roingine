input.onKeyDown(input.KEY_A, function() {
    roingine.print("A pressed");
});
input.onKeyUp(input.KEY_A, function() {
    roingine.print("A released");
});
input.onKeyHeld(input.KEY_A, function() {
    roingine.print("A held");
});
input.onKeyLongPressed(input.KEY_A, function() {
    roingine.print("A long pressed");
});

function Update(deltaTime) {
    roingine.print("DeltaTime:", deltaTime);
}
