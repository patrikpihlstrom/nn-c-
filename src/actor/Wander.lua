State_Wander = {}

State_Wander["Enter"] = function(actor)
	print("Entering wander state");
end

State_Wander["Execute"] = function(actor)
	if math.sqrt(math.pow(actor:getTarget().x - actor:getPosition().x, 2) + math.pow(actor:getTarget().y - actor:getPosition().y, 2)) <= 10 then
		target = {x = math.random(-500, 500), y = math.random(-500, 500)};
		angle = math.atan2(target.y - actor:getPosition().y, target.x - actor:getPosition().x);
		actor:assignTarget(target.x, target.y);
		actor:setVelocity(math.cos(angle)*200, math.sin(angle)*200);
	end
end

State_Wander["Exit"] = function(actor)
	print("Exiting wander state");
end

