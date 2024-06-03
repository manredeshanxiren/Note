let myHeading = document.querySelector("h1");
myHeading.textContent = "Hello world!";

// let iceCream = "chocolate";
// if (iceCream === "chocolate") {
//   alert("我最喜欢巧克力冰淇淋了。");
// } else {
//   alert("但是巧克力才是我的最爱呀……");
// }


// document.querySelector("img").addEventListener("click", function () {
//     alert("别戳我，我怕疼。");
//   });
  


let myImage = document.querySelector("img");

myImage.onclick = function () {
  let mySrc = myImage.getAttribute("src");
  if (mySrc === "image/enwiki-2x.png") {
    myImage.setAttribute("src", "image/270px-WiktionaryEn.svg.png");
  } else {
    myImage.setAttribute("src", "image/enwiki-2x.png");
  }
};

let myButton = document.querySelector("button");


function setUserName() {
    let myName = prompt("请输入你的名字。");
    localStorage.setItem("name", myName);
    myHeading.textContent = "damn bro，" + myName;
  }
  

  if (!localStorage.getItem("name")) {
    setUserName();
  } else {
    let storedName = localStorage.getItem("name");
    myHeading.textContent = "damn bro，" + storedName;
  }
  
  myButton.onclick = function () {
    setUserName();
  };
  
