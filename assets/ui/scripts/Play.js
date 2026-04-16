"use strict";
const form = document.querySelector("form");
const input = document.querySelector("input");
const submit = document.querySelector("button[type=submit]");
form.addEventListener("submit", (e) => {
    e.preventDefault();
    Send("Play", input.value);
});
input.addEventListener("input", (e) => {
    const target = e.target;
    submit.disabled = target.value === "";
});
