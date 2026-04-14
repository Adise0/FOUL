const points = document.querySelector(".points");

window.bridge.on("Points", (newPoints) => {
  points.textContent = newPoints;
});
