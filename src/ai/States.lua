
-- IDLE

State_Idle = {timer = 0}

State_Idle["Enter"] = function(actor)
	State_Idle.timer = 0;
end

State_Idle["Execute"] = function(actor)
	State_Idle.timer = State_Idle.timer + 1;

	if State_Idle.timer > 120 then
		actor:changeState(State_Wander);
	end
end

State_Idle["Exit"] = function(actor)
end


-- WANDER 

State_Wander = {target = {x = 0, y = 0}}

State_Wander["Enter"] = function(actor)
	State_Wander.target.x = math.random(-500, 500);
	State_Wander.target.y = math.random(-500, 500);
	angle = math.atan2(State_Wander.target.y - actor:getPositionY(), State_Wander.target.x - actor:getPositionX());
	x = math.cos(angle)*50;
	y = math.sin(angle)*50;
	actor:setVelocity(x, y);
end

State_Wander["Execute"] = function(actor)
	if math.sqrt(math.pow(State_Wander.target.x - actor:getPositionX(), 2) + math.pow(State_Wander.target.y - actor:getPositionY(), 2)) < 10 then
		actor:setVelocity(0, 0);
		actor:changeState(State_Idle);
	end
end

State_Wander["Exit"] = function(actor)
end

