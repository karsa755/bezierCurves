var canvas = document.getElementById("canv");
var context = canvas.getContext("2d");
var ctrlPoints = [];
var height = canvas.height = window.innerHeight;
var width = canvas.width = window.innerWidth;
var tessEps = 2.0;
var mouseClicked = false, mouseReleased = true;
ctrlPoints.push({x:0, y:0});
ctrlPoints.push({x:200, y:200});
ctrlPoints.push({x:600, y:0});
ctrlPoints.push({x:1000, y:200});

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
plotBezier(ctrlPoints);
//document.addEventListener("click", onMouseClick, false);
//document.addEventListener("mousemove", onMouseMove, false);

function onMouseClick(e) {
    ctrlPoints.push({x:e.clientX, y:e.clientY});
    context.fillRect(e.clientX,e.clientY,10,10); 
    console.log(ctrlPoints);
    mouseClicked = !mouseClicked;
}

function maxDistance(A) {
    var baselineX = A[A.length-1].x - A[0].x;
    var baselineY = A[A.length-1].y - A[0].y;
    var maxH = -1;

    for(var i = 0;  i < A.length; ++i)
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

function midSubdivide(bez,leftBez, rightBez, t) {
    
    console.log(leftBez);
    leftBez[0].x = bez[0].x;
	leftBez[0].y = bez[0].y;
	rightBez[bez.length-1].x = bez[bez.length-1].x;
	rightBez[bez.length-1].y = bez[bez.length-1].y;

    for(var k = 1; k < bez.length; ++k)
    {
        for(i = 0; i < bez.length-k; ++i)
        {
			//basically de Casteljau algorithm
			bez[i].x = (1 - t) * bez[i].x + t*bez[i + 1].x;
			bez[i].y = (1 - t) * bez[i].y + t*bez[i + 1].y;
        }
        		//subdivision of the curve
		leftBez[k].x = bez[0].x;
		leftBez[k].y = bez[0].y;
		rightBez[bez.length-1 - k].x = bez[bez.length-1 - k].x;
		rightBez[bez.length-1 - k].y = bez[bez.length-1 - k].y;
    }
}

function plotBezier(bez)
{
    var height = maxDistance(bez);
    leftBez = [];
    rightBez = [];
    for(var i = 0; i < bez.length; ++i)
    {
        leftBez.push({x:0, y:0});
        rightBez.push({x:0, y:0});
    }

    if(height < tessEps)
    {
        context.fillRect(bez[0].x, bez[0].y,10,10);
        context.fillRect(bez[bez.length-1].x, bez[bez.length-1].y,10,10); 

        context.beginPath();
        context.moveTo(bez[0].x, bez[0].y);
        context.lineTo(bez[bez.length-1].x, bez[bez.length-1].y);
        context.stroke();

    }
    else
    {
        midSubdivide(bez, leftBez, rightBez);
        plotBezier(leftBez);
        plotBezier(rightBez);
    }

}

function onMouseMove(e) {
    if (mouseClicked) {

    }
}