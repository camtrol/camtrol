let target = document.querySelector("#dynamic");
let stringArr = ["Learn to HTML", "Learn to CSS",
    "Learn to JavaScript", "Learn to Python", "Learn to Ruby"];
let selectString = stringArr[Math.floor(random() * stringArr.length)];
let selectStringArr = selectString.split("");

function dynamic(randomArr) {
    if (randomArr.length > 0) {
        target.textContent += randomArr.shift();
    }
}

dynamic(selectStringArr);

console.log(selectStringArr);

function blink() {
    target.classList.toggle("active");
};
setInterval(blink, 500);