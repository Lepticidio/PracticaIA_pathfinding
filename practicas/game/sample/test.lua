MOAISim.openWindow("game", 512, 512)

viewport = MOAIViewport.new()
viewport:setSize (512, 512)
viewport:setScale (512, -512)
viewport:setOffset (-1, 1)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

texture_name = "ghost32.png"
gfxQuad = MOAIGfxQuad2D.new()
gfxQuad:setTexture(texture_name)
gfxQuad:setRect(0, 0, 32, 32)
gfxQuad:setUVRect(0, 0, 1, 1)

prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)

entity = Character.new()
-- Add prop to be the renderable for this character
entity:setProp(prop, layer)
-- Start the character (allow calls to OnUpdate)
entity:start()
entity:setLoc(64, 160)
entity:setRot(0)
entity:setLinearVel(0, 0)
entity:setAngularVel(0)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)


pathfinder = Pathfinder.new()
pathfinder:setStartPosition(64, 160)
pathfinder:setEndPosition(64, 224)
MOAIDrawDebug.insertEntity(pathfinder)
-- Add this character to draw debug
MOAIDrawDebug.insertEntity(entity)

mouseX = 0
mouseY = 0


function onClick(down)
  pathfinder:setStartPosition(mouseX, mouseY)
end

function onRightClick(down)
  pathfinder:setEndPosition(mouseX, mouseY)
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.mouseRight:setCallback(onRightClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)

function onKeyPressed(key, down)
	if key == 32 then
		if down then
			print(tostring(key))
		else
			pathfinder:pathfindStep()
		end
	end
	if key == 13 then
		if down then
			print(tostring(key))
		else
			pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()

    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
    		pathfinder:pathfindStep()
		end
	end
end
if (MOAIInputMgr.device.keyboard) then
    MOAIInputMgr.device.keyboard:setCallback(onKeyPressed)
end
