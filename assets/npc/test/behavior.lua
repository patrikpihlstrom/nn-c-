
-- INIT

State_Init = {}

State_Init["Enter"] = function(actor)
	actor:setHealth(100);
	actor:setSize(math.random(4, 64));
	actor:setPosition(math.random(0, 200)*10, 0);
	actor:changeState(State_Wander);
end

State_Init["Execute"] = function(actor)
end

State_Init["Exit"] = function(actor)
end

-- IDLE

State_Idle = {timer = 0, limit = math.random(120, 480)}

State_Idle["Enter"] = function(actor)
	State_Idle.timer = 0;
end

State_Idle["Execute"] = function(actor)
	State_Idle.timer = State_Idle.timer + 1;

	if State_Idle.timer > State_Idle.limit  then
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
end

State_Wander["Execute"] = function(actor)
	distance = math.sqrt(math.pow(State_Wander.target.x - actor:getPositionX(), 2) + math.pow(State_Wander.target.y - actor:getPositionY(), 2));

	if distance <= 10 then
		actor:setVelocity(0, 0);
		actor:changeState(State_Idle);
	else
		x = math.cos(angle)*distance;
		y = math.sin(angle)*distance;
		actor:setVelocity(x, y);
	end
end

State_Wander["Exit"] = function(actor)
end

