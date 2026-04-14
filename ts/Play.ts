const form = document.querySelector<HTMLFormElement>("form");
const input = document.querySelector<HTMLInputElement>("input");
const submit = document.querySelector<HTMLButtonElement>("button[type=submit]");

form.addEventListener("submit", (e: Event) => {
  e.preventDefault();
  console.log("Form submitted!");
});

input.addEventListener("input", (e) => {
  const target = e.target as HTMLInputElement;
  submit.disabled = target.value === "";
});
