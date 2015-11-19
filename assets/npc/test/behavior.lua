
-- INIT

State_Init = {origin = {x = 0, y = 0}}

State_Init["Enter"] = function(actor)
	actor:setHealth(100);
	width = 16;--math.random(4, 32);
	height = 72;--width*(math.random(20, 45)/10);
	actor:setSize(width, height);
	actor:changeState(State_Wander);
	State_Init.origin.x = actor:getPositionX();
	State_Init.origin.y = actor:getPositionY();
end

State_Init["Execute"] = function(actor)
end

State_Init["Exit"] = function(actor)
end

-- IDLE

State_Idle = {timer = math.random(20, 50)}

State_Idle["Enter"] = function(actor)
	State_Idle.timer = math.random(20, 50);
end

State_Idle["Execute"] = function(actor)
	State_Idle.timer = State_Idle.timer - 1;

	if State_Idle.timer <= 0  then
		actor:changeState(State_Wander);
	end
end

State_Idle["Exit"] = function(actor)
end


-- WANDER 

State_Wander = {target = {x = 0, y = 0}, angle = 0}

State_Wander["Enter"] = function(actor)
	if actor:hasPlayer() then
		playerX = actor:getPlayerPositionX();
		playerY = actor:getPlayerPositionY();

		if math.sqrt(math.pow(playerX - actor:getPositionX(), 2) + math.pow(playerY - actor:getPositionY(), 2)) <= 500 then
			actor:changeState(State_Follow_Player);
		end
	end

	State_Wander.target.x = math.random(-100, 100) + actor:getPositionX();
	State_Wander.target.y = math.random(-100, 100) + actor:getPositionY();
	State_Wander.angle = math.atan2(State_Wander.target.y - actor:getPositionY(), State_Wander.target.x - actor:getPositionX());
end

State_Wander["Execute"] = function(actor)
	distance = math.sqrt(math.pow(State_Wander.target.x - actor:getPositionX(), 2) + math.pow(State_Wander.target.y - actor:getPositionY(), 2));

	while math.sqrt(math.pow(State_Init.origin.x - State_Wander.target.x, 2) + math.pow(State_Init.origin.y - State_Wander.target.y, 2)) > 150 do
		State_Wander.target.x = math.random(-100, 100) + actor:getPositionX();
		State_Wander.target.y = math.random(-100, 100) + actor:getPositionY();
		State_Wander.angle = math.atan2(State_Wander.target.y - actor:getPositionY(), State_Wander.target.x - actor:getPositionX());
	end

	if distance <= 2 then
		actor:setVelocity(0, 0);
		actor:changeState(State_Idle);
	else
		x = math.cos(State_Wander.angle)*200;
		y = math.sin(State_Wander.angle)*200;
		actor:setVelocity(x, y);
	end
end

State_Wander["Exit"] = function(actor)
	State_Wander.target.x = 0;
	State_Wander.target.y = 0;
	State_Wander.target.angle = 0;
end

-- FOLLOW PLAYER

State_Follow_Player = {hasPlayer = false}

State_Follow_Player["Enter"] = function(actor)
	actor:setVelocity(0, 0);
	if actor:hasPlayer() then
		State_Follow_Player.hasPlayer = true;
	else
		State_Follow_Player.hasPlayer = false;
	end
end

State_Follow_Player["Execute"] = function(actor)
	if State_Follow_Player.hasPlayer then
		playerX = actor:getPlayerPositionX();
		playerY = actor:getPlayerPositionY();

		--if math.sqrt(math.pow(playerX - actor:getPositionX(), 2) + math.pow(playerY - actor:getPositionY(), 2)) >= 1000 then
		--	actor:setVelocity(0, 0);
		--	actor:changeState(State_Idle);
		--else
			angle = math.atan2(playerY - actor:getPositionY(), playerX - actor:getPositionX());
			actor:setVelocity(math.cos(angle)*200, math.sin(angle)*200);
		--end
	else
		actor:changeState(State_Idle);
	end
end

State_Follow_Player["Exit"] = function(actor)
	State_Follow_Player.hasPlayer = false;
end

