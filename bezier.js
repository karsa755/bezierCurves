var canvas = document.getElementById("canv");
var context = canvas.getContext("2d");
var ctrlPoints = [];
var height = canvas.height = window.innerHeight;
var width = canvas.width = window.innerWidth;

var mouseClicked = false, mouseReleased = true;

document.addEventListener("click", onMouseClick, false);
document.addEventListener("mousemove", onMouseMove, false);

function onMouseClick(e) {
    ctrlPoints.push({x:e.clientX, y:e.clientY});
    context.fillRect(e.clientX,e.clientY,10,10); 
    console.log(ctrlPoints);
    mouseClicked = !mouseClicked;
}
function decasteljau(A, t) {
    Q = [];

    for(var i = 0; i < A.length; ++i)
    {
        Q.push(A[i]); 
    }

    for(var k = 1; k < A.length; ++k)
    {
        for(i = 0; i < A.length-k; ++i)
        {
            Q[i] = (1-t) * Q[i] + t * Q[i+1];
        }
    }
}
function onMouseMove(e) {
    if (mouseClicked) {

    }
}