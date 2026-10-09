// The test's windows shown again, as the task bar would.
for (const w of workspace.windowList()) {
    if (w.resourceClass == "caustic-media") { w.minimized = false; }
}
