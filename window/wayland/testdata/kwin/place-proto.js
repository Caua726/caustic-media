// The protocols test's windows where the pointer and the screen readback
// will find them: proto-a at the top left, proto-b to its right. Moved only:
// a new size would wait for the client to draw it.
for (const w of workspace.windowList()) {
    const g = w.frameGeometry;
    if (w.caption == "proto-a") { w.frameGeometry = { x: 0, y: 0, width: g.width, height: g.height }; }
    if (w.caption == "proto-b") { w.frameGeometry = { x: 500, y: 0, width: g.width, height: g.height }; }
}
