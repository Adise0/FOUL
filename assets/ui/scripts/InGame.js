"use strict";
{
    const points = document.querySelector(".points");
    const respawn = document.querySelector(".respawn");
    const holder = document.querySelector(".holder");
    window.bridge.on("Points", (newPoints) => {
        points.textContent = newPoints;
    });
    window.bridge.on("Respawn", (content) => {
        if (content == "0") {
            holder.style.background = "none";
            respawn.style.opacity = "0";
        }
        else {
            holder.style.backgroundColor = "rgba(0,0,0,0.4)";
            respawn.style.opacity = "1";
        }
        respawn.textContent = content;
        respawn.animate([
            { transform: "translate(20px, -10px) rotateZ(20deg)" },
            { transform: "translate(0, 0) rotateZ(0deg)" },
        ], {
            duration: 500,
            easing: "cubic-bezier(0.38, 0.22, 0.29, 2.05)",
            fill: "forwards",
        });
    });
}
