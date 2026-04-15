{
  const points = document.querySelector<HTMLParagraphElement>(".points")!;
  const pointsFinal = document.querySelector<HTMLParagraphElement>("h1")!;
  const respawn = document.querySelector<HTMLParagraphElement>(".respawn")!;
  const holder = document.querySelector<HTMLDivElement>(".holder")!;

  const content = document.querySelector<HTMLDivElement>(".content")!;
  const personalBest = document.querySelector<HTMLDivElement>(".personalBest")!;
  const dude = document.querySelector<HTMLDivElement>(".dude")!;

  window.bridge.on("Points", (newPoints) => {
    points.textContent = newPoints ?? "000";
  });

  window.bridge.on("Respawn", (content) => {
    if (content == "0") {
      holder.style.background = "none";
      respawn.style.opacity = "0";
    } else {
      holder.style.backgroundColor = "rgba(0,0,0,0.4)";
      respawn.style.opacity = "1";
    }

    respawn.textContent = content ?? "";
    respawn.animate(
      [
        { transform: "translate(20px, -10px) rotateZ(20deg)" },
        { transform: "translate(0, 0) rotateZ(0deg)" },
      ],
      {
        duration: 500,
        easing: "cubic-bezier(0.38, 0.22, 0.29, 2.05)",
        fill: "forwards",
      },
    );
  });

  interface GameOverData {
    name: string;
    points: string;
    isPersonalBest: boolean;
  }

  window.bridge.on("GameOver", (payload) => {
    const data: GameOverData = JSON.parse(payload as string);

    console.log(JSON.stringify(data));

    holder.style.backgroundColor = "rgba(0,0,0, 0.4)";
    content.style.opacity = "1";
    personalBest.style.opacity = data.isPersonalBest ? "1" : "0";
    dude.textContent = data.name;
    pointsFinal.textContent = data.points;
  });
}
