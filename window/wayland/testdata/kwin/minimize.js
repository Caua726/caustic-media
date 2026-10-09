// The test's windows minimized, as their title bar's button would.
for (const w of workspace.windowList()) {
    if (w.resourceClass == "caustic-media") { w.minimized = true; }
}
