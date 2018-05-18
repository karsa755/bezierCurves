var canvas = document.getElementById("canv");
var context = canvas.getContext("2d");
var ctrlPoints = [];
var deg;
var height = canvas.height = window.innerHeight;
var width = canvas.width = window.innerWidth;
var tessEps = 2;
var mouseClicked = false, mouseReleased = true;

/* ctrlPoints.push({x:0, y:0});
ctrlPoints.push({x:430, y:520});
ctrlPoints.push({x:1000, y:00});
ctrlPoints.push({x:300, y:250});
var newDeg = ctrlPoints.length -1;
for(var i = 0; i < ctrlPoints.length; ++i)
{
    context.fillRect(ctrlPoints[i].x,ctrlPoints[i].y,10,10);
}
for(var i = 0; i < ctrlPoints.length; ++i)
{

    var newCurve = [];
    var T0X = 0;
    var T1X = 0;
    var T0Y = 0;
    var T1Y = 0;
    if(i == 0)
    {   
        T0X = 0.5 * ctrlPoints[i+1].x;
        T0Y = 0.5 * ctrlPoints[i+1].y;

        T1X = -0.5 * (ctrlPoints[i+2].x - ctrlPoints[i].x);
        T1Y = -0.5 * (ctrlPoints[i+2].y - ctrlPoints[i].y);
        //newCurve.push(ctrlPoints[i],  )
        console.log(T0X);
    }
    else if(i == ctrlPoints.length -2)
    {
        T0X = 0.5 * (ctrlPoints[i+1].x - ctrlPoints[i-1].x);
        T0Y = 0.5 * (ctrlPoints[i+1].y - ctrlPoints[i-1].y);

        T1X = 0.5 * ctrlPoints[i].x;
        T1Y = 0.5 * ctrlPoints[i].y;
    }
    else if(i == ctrlPoints.length -1)
    {

    }
    else
    {
        T0X = 0.5 * (ctrlPoints[i+1].x - ctrlPoints[i-1].x);
        T0Y = 0.5 * (ctrlPoints[i+1].y - ctrlPoints[i-1].y);
        T1X = -0.5 * (ctrlPoints[i+2].x - ctrlPoints[i].x);
        T1Y = -0.5 * (ctrlPoints[i+2].y - ctrlPoints[i].y);
    }
    
    if(i < 3)
    {

        newCurve.push(ctrlPoints[i]);
        newCurve.push({x:ctrlPoints[i].x + (1/3) *T0X, y:ctrlPoints[i].y + (1/3) *T0Y});
        newCurve.push({x:ctrlPoints[i+1].x + (1/3) *T1X, y:ctrlPoints[i+1].y + (1/3) *T1Y});
        newCurve.push(ctrlPoints[i+1]);
        //plotBezier(newCurve, 3);
        console.log(newCurve);
    }
    
} */

document.addEventListener("click", onMouseClick, false);
document.addEventListener("mousemove", onMouseMove, false);

function onMouseClick(e) {
    ctrlPoints.push({x:e.clientX, y:e.clientY});

    deg = ctrlPoints.length-1;
    context.fillRect(e.clientX,e.clientY,10,10);
    
    if(ctrlPoints.length > 1)
    {
        context.clearRect(0, 0, width, height);
        plotCtrlPoints(ctrlPoints,deg);
        plotBezier(ctrlPoints,deg);
    }
    mouseClicked = !mouseClicked;
}


function onMouseMove(e) {
    if (mouseClicked) {

    }
}

function maxDistance(A, k) {
    
    var baselineX = JSON.parse(JSON.stringify(A[k].x - A[0].x));
    var baselineY = JSON.parse(JSON.stringify(A[k].y - A[0].y));

    
    var maxH = -1;

    for(var i = 0;  i < k+1; ++i)
    {
        var seclineX = JSON.parse(JSON.stringify(A[i].x - A[0].x));
        var seclineY = JSON.parse(JSON.stringify(A[i].y - A[0].y));
        //cross product of secline
        var cross = ((baselineX * seclineY) - (baselineY * seclineX));
        var h = Math.abs(cross) / Math.sqrt(baselineX*baselineX + baselineY*baselineY);
        
        if(maxH < h)
        {
            maxH = h;
        }
    }
    
    return maxH;
}

function midSubdivide(Originalbez,leftBez, rightBez, t=0.5, deg) {
    
    var bez = JSON.parse(JSON.stringify(Originalbez));
    leftBez[0].x = bez[0].x;    
	leftBez[0].y = bez[0].y;
	rightBez[deg].x = bez[deg].x;
    rightBez[deg].y = bez[deg].y;
    
    
    for(var k = 1; k < deg+1; ++k)
    {
        for(i = 0; i < deg+1-k; ++i)
        {
			//basically de Casteljau algorithm
			bez[i].x =  ((1 - t) * bez[i].x + t*bez[i + 1].x);
			bez[i].y =  ((1 - t) * bez[i].y + t*bez[i + 1].y);
        }
        		//subdivision of the curve
		leftBez[k].x = bez[0].x;
		leftBez[k].y = bez[0].y;
		rightBez[deg - k].x = bez[deg - k].x;
		rightBez[deg - k].y = bez[deg - k].y;
    }
    
}

function plotBezier(bez, k,x,y)
{
    var leftBez = [];
    var rightBez = [];
    for(var i = 0; i < k+2; ++i)
    {
        leftBez.push({x:0, y:0});
        rightBez.push({x:0,y:0});
    }

    var height = maxDistance(bez, k);
    if(height < tessEps)
    {     
        context.fillRect(bez[k].x, bez[k].y,5,5); 

        context.beginPath();
        context.moveTo(bez[0].x, bez[0].y);
        context.lineTo(bez[k].x, bez[k].y);
        context.stroke();

    }
    else
    {
        midSubdivide(bez, leftBez, rightBez, 0.5, k);
        plotBezier(leftBez, k);
        plotBezier(rightBez, k);
    }

}

function plotCtrlPoints(bez, k)
{
    for(var i = 0; i < k+1; ++i)
    {
        context.fillRect(bez[i].x,bez[i].y,10,10); 
    }
}

function clearCanvas(){
    var c = document.getElementById('canv');
    var ctx = c.getContext('2d');
    ctx.clearRect(0, 0, width, height);
   }

   document.addEventListener('keydown', function(event) {
    if(event.keyCode == 67) {
        ctrlPoints = [];
        clearCanvas();
    }
    if(event.keyCode == 83) {
        ctrlPoints = [];
    }
});