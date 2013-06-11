"use strict"

var computeVoronoiEdges = function(sites) {
	var sites_array = _malloc(16 * sites.length)
	for(var sitei = 0; sitei < sites.length; ++sitei) {
		setValue(sites_array + 16 * sitei, sites[sitei][0], "double")
		setValue(sites_array + 16 * sitei + 8, sites[sitei][1], "double")
	}
	var voronoi = _frivoldraw_ComputeVoronoi(sites_array, sites.length)
	var edges = []
	var edge_count = _frivoldraw_GetEdgeCount(voronoi)
	for(var edgei = 0; edgei < edge_count; ++edgei) {
		edges.push([
			[_frivoldraw_GetStartX(voronoi, edgei), _frivoldraw_GetStartY(voronoi, edgei)],
			[_frivoldraw_GetEndX(voronoi, edgei), _frivoldraw_GetEndY(voronoi, edgei)],
		])
	}
	_frivoldraw_FreeVoronoi(voronoi)
	_free(sites_array)
	return edges
}

$(document).ready(function() {
	var max_sites = 500
	var sites = []
	var voronoi_edges = []
	
	var canvas = $("<canvas />")
		.attr("width", window.innerWidth)
		.attr("height", window.innerHeight)
		.css("position", "fixed")
		.css("top", "0px")
		.css("left", "0px")
	var ctx = canvas[0].getContext("2d")
	$("body").append(canvas)
	
	var redraw = function() {
		var width = canvas[0].width
		var height = canvas[0].height
		ctx.clearRect(0, 0, width, height)
		
		for(var sitei = 0; sitei < sites.length; ++sitei) {
			ctx.beginPath()
			ctx.arc(sites[sitei][0], sites[sitei][1], 3, 0, 2 * Math.PI)
			ctx.fill()
		}
		
		for(var edgei = 0; edgei < voronoi_edges.length; ++edgei) {
			var edge = voronoi_edges[edgei]
			ctx.beginPath()
			ctx.moveTo(edge[0][0], edge[0][1])
			ctx.lineTo(edge[1][0], edge[1][1])
			ctx.stroke()
		}
	}
	
	var sitesChanged = function() {
		voronoi_edges = computeVoronoiEdges(sites)
		redraw()
	}
	
	var dragSite = function(x, y) {
		// Find closest site and move it to mouse position.
		var best = Infinity
		var besti = -1
		for(var sitei = 0; sitei < sites.length; ++sitei) {
			var dx = sites[sitei][0] - x
			var dy = sites[sitei][1] - y
			var dist2 = dx * dx + dy * dy
			if(dist2 < best) {
				best = dist2
				besti = sitei
			}
		}
		if(besti != -1) {
			sites[besti] = [x + Math.random(), y + Math.random()]
		}
		sitesChanged()
	}
	
	var mouse_down = false
	canvas.on("mousedown", function(event) {
		mouse_down = true
		dragSite(event.pageX, event.pageY)
	})
	canvas.on("mousemove", function(event) {
		if(mouse_down) {
			dragSite(event.pageX, event.pageY)
		}
	})
	canvas.on("mouseout", function() {
		mouse_down = false
	})
	canvas.on("mouseup", function() {
		mouse_down = false
	})
	
	var selector = $("<input />")
		.attr("type", "number")
		.attr("min", "0")
		.attr("max", max_sites)
		.attr("value", "30")
		.css("position", "fixed")
		.css("top", "0px")
		.css("left", "0px")
	$("body").append(selector)
	
	var siteCountChange = function() {
		var value = parseInt(selector[0].value)
		if(value.toString() != selector[0].value || value < 0 || value > max_sites) {
			value = sites.length
			selector[0].value = value.toString()
		}
		
		while(sites.length < value) {
			sites.push([canvas[0].width * Math.random(), canvas[0].height * Math.random()])
		}
		while(sites.length > value) {
			sites.pop()
		}
		
		sitesChanged()
	}
	selector.on("change", siteCountChange)
	siteCountChange()
	
	var windowResize = function() {
		canvas[0].width = window.innerWidth
		canvas[0].height = window.innerHeight
		redraw()
	}
	$(window).on("resize", windowResize)
	windowResize()
})
