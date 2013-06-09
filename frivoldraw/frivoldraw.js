"use strict"

$(document).ready(function() {
	var max_sites = 500
	var sites = []
	
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
	}
	
	var dragPoint = function(x, y) {
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
			sites[besti] = [x, y]
		}
		redraw()
	}
	
	var mouse_down = false
	canvas.on("mousedown", function(event) {
		mouse_down = true
		dragPoint(event.pageX, event.pageY)
	})
	canvas.on("mousemove", function(event) {
		if(mouse_down) {
			dragPoint(event.pageX, event.pageY)
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
		.attr("value", "6")
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
		
		redraw()
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
