// Two windows of a drag test where the pointer will find them: ours at the
// left of the screen, the GTK peer's to its right. Moved only: a new size
// would wait for the client to draw it before KWin moved the window either.
for (const w of workspace.windowList()) {
    const g = w.frameGeometry;
    if (w.caption == "dnd-ours") { w.frameGeometry = { x: 0, y: 0, width: g.width, height: g.height }; }
    if (w.caption == "dnd-peer") { w.frameGeometry = { x: 600, y: 100, width: g.width, height: g.height }; }
}
