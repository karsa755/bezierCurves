var canvas = document.getElementById("canv");
var context = canvas.getContext("2d");
var ctrlPoints = [];
var height = canvas.height = window.innerHeight;
var width = canvas.width = window.innerWidth;
var tessEps = 100;
var mouseClicked = false, mouseReleased = true;
ctrlPoints.push({x:100, y:100});
ctrlPoints.push({x:200, y:200});
ctrlPoints.push({x:600, y:0});
ctrlPoints.push({x:1000, y:200});
var deg = ctrlPoints.length - 2;

for(var i = 0; i < ctrlPoints.length; ++i)
{
    context.fillRect(ctrlPoints[i].x, ctrlPoints[i].y, 10, 10);
    if(i != ctrlPoints.length-1)
    {
        //ontext.beginPath();
        //context.moveTo(ctrlPoints[i].x, ctrlPoints[i].y);
        //context.lineTo(ctrlPoints[i+1].x, ctrlPoints[i+1].y);
        //context.stroke();
    }
}

plotBezier(ctrlPoints, deg);
//document.addEventListener("click", onMouseClick, false);
//document.addEventListener("mousemove", onMouseMove, false);

function onMouseClick(e) {
    ctrlPoints.push({x:e.clientX, y:e.clientY});
    context.fillRect(e.clientX,e.clientY,10,10); 
   
    mouseClicked = !mouseClicked;
}

function maxDistance(A, k) {
    
    var baselineX = A[k+1].x - A[0].x;
    var baselineY = A[k+1].y - A[0].y;
 
    var maxH = -1;

    for(var i = 0;  i < k+2; ++i)
    {
        var seclineX = A[i].x - A[0].x;
        var seclineY = A[i].y - A[0].y;
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

function midSubdivide(bez,leftBez, rightBez, t, deg) {
    
    
    leftBez[0].x = bez[0].x;    
	leftBez[0].y = bez[0].y;
	rightBez[deg+1].x = bez[deg+1].x;
	rightBez[deg+1].y = bez[deg+1].y;

    for(var k = 1; k < deg+2; ++k)
    {
        for(i = 0; i < deg+2-k; ++i)
        {
			//basically de Casteljau algorithm
			bez[i].x = (1 - t) * bez[i].x + t*bez[i + 1].x;
			bez[i].y = (1 - t) * bez[i].y + t*bez[i + 1].y;
        }
        		//subdivision of the curve
		leftBez[k].x = bez[0].x;
		leftBez[k].y = bez[0].y;
		rightBez[deg+2 - k].x = bez[deg+2 - k].x;
		rightBez[deg+2 - k].y = bez[deg+2 - k].y;
    }
    
}

function plotBezier(bez, k)
{
    console.log(bez);
    var leftBez = [];
    var rightBez = [];
    for(var i = 0; i < deg+2; ++i)
    {
        leftBez.push({x:0, y:0});
        rightBez.push({x:0,y:0});
    }

    var height = maxDistance(bez, k);
    
    if(height < tessEps)
    {
       
        
        context.fillRect(bez[k+1].x, bez[k+1].y,2,2); 

        context.beginPath();
        context.moveTo(bez[0].x, bez[0].y);
        //context.moveTo(100, 100);
        context.lineTo(bez[k+1].x, bez[k+1].y);
        context.stroke();

    }
    else
    {
        midSubdivide(bez, leftBez, rightBez, 0.5, k);
        plotBezier(leftBez, k);
        plotBezier(rightBez, k);
    }

}

function onMouseMove(e) {
    if (mouseClicked) {

    }
}

