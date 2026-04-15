{
  let isShowing = false;

  const holder = document.querySelector<HTMLDivElement>(".holder")!;
  const paused = document.querySelector<HTMLDivElement>(".paused")!;

  window.bridge.on("Disable", () => {
    holder.style.opacity = "0";
    isShowing = false;
  });

  window.bridge.on("Enable", (payload) => {
    holder.style.opacity = "1";
    isShowing = true;

    const isTutorial = payload === "true";
    if (!isTutorial) paused.style.opacity = "1";
    else paused.style.opacity = "0";
  });

  window.addEventListener("keydown", (e: KeyboardEvent) => {
    if (e.keyCode !== 27 || !isShowing) return;
    Send("Resume");
  });
}
