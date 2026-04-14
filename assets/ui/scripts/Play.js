"use strict";
const form = document.querySelector("form");
const input = document.querySelector("input");
const submit = document.querySelector("button[type=submit]");
form.addEventListener("submit", (e) => {
    e.preventDefault();
    console.log("Form submitted!");
});
input.addEventListener("input", (e) => {
    const target = e.target;
    submit.disabled = target.value === "";
});
