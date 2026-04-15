const quitBtn = document.querySelector(".back")!;

quitBtn!.addEventListener("click", () => Send("__quit"));

const leaderboardLink =
  document.querySelector<HTMLAnchorElement>("#leaderboard")!;

leaderboardLink.addEventListener("click", () =>
  window.bridge.emit("Leaderboard"),
);

interface Player {
  name: string;
  points: number;
}

window.bridge.on("Leaderboard", (payload) => {
  const data: Player[] = JSON.parse(payload);

  // TODO: inject
});
