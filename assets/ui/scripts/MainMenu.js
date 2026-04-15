"use strict";
const quitBtn = document.querySelector(".back");
quitBtn.addEventListener("click", () => Send("__quit"));
const leaderboardLink = document.querySelector("#leaderboard");
leaderboardLink.addEventListener("click", () => window.bridge.emit("Leaderboard"));
window.bridge.on("Leaderboard", (payload) => {
    const data = JSON.parse(payload);
    // TODO: inject
});
