// The selections test's window where the pointer will find it: the top left
// of the screen. Moved only: a new size would wait for the client to draw it.
for (const w of workspace.windowList()) {
    const g = w.frameGeometry;
    if (w.caption == "selections") { w.frameGeometry = { x: 0, y: 0, width: g.width, height: g.height }; }
}
