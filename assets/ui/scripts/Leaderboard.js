"use strict";
const container = document.querySelector(".container");
let isLoaded = false;
const token = setInterval(() => {
    if (isLoaded) {
        clearInterval(token);
    }
    console.log("Requesting leaderboard!");
    Send("Leaderboard");
}, 100);
window.bridge.on("Leaderboard", (payload) => {
    isLoaded = true;
    const data = JSON.parse(payload);
    console.log("Received data! " + data.length);
    data.sort((a, b) => {
        if (a.points == b.points)
            return 0;
        else if (a.points > b.points)
            return -1;
        else
            return 1;
    });
    let content = "";
    let counter = 0;
    data.forEach((player) => {
        counter++;
        let other = "";
        if (counter <= 3) {
            switch (counter) {
                case 1:
                    other = "gold";
                    break;
                case 2:
                    other = "silver";
                    break;
                case 3:
                    other = "bronze";
                    break;
            }
        }
        content += `
      <div class="item ${other}" >
        <p class="name">${player.name}</p>
        <p class="points">${player.points}</p>
      </div>
    `;
        container.innerHTML = content;
    });
});
