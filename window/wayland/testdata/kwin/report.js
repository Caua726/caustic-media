// What KWin knows of each window, a line each in its log (js category):
// REPORT caption|the caption of the window it is a child of|1 when it has
// the keyboard|1 when it has an icon of its own|its frame's x,y
for (const w of workspace.windowList()) {
    const parent = w.transientFor ? w.transientFor.caption : "";
    const active = workspace.activeWindow === w ? 1 : 0;
    const icon = w.icon && !w.icon.isNull ? 1 : 0;
    print("REPORT " + w.caption + "|" + parent + "|" + active + "|" + icon + "|" +
          Math.round(w.frameGeometry.x) + "," + Math.round(w.frameGeometry.y));
}
