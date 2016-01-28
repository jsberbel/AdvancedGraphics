#include "BallController.h"
#include "Grid.h"

void BallController::updateBalls(std::vector <Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY) {
    const float FRICTION = 0.001f;
    // Update our grabbed balls velocity
    if (m_grabbedBall != -1) {
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
    }

    glm::vec2 gravity = getGravityAccel();

    for (size_t i = 0; i < balls.size(); i++) {
        // get handle for less typing
        Ball& ball = balls[i];
        // Update motion if its not grabbed
        if (i != m_grabbedBall) {
            ball.position += ball.velocity * deltaTime;
            // Apply friction
            glm::vec2 momentumVec = ball.velocity * ball.mass;
            if (momentumVec.x != 0 || momentumVec.y != 0) {
                if (FRICTION < glm::length(momentumVec)) {
                    ball.velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / ball.mass;
                } else {
                    ball.velocity = glm::vec2(0.0f);
                }
            }
            // Apply gravity
            ball.velocity += gravity * deltaTime;
        }
        // Check wall collision
        if (ball.position.x < ball.radius) {
            ball.position.x = ball.radius;
            if (ball.velocity.x < 0) ball.velocity.x *= -1;
        } else if (ball.position.x + ball.radius >= maxX) {
            ball.position.x = maxX - ball.radius - 1;
            if (ball.velocity.x > 0) ball.velocity.x *= -1;
        }
        if (ball.position.y < ball.radius) {
            ball.position.y = ball.radius;
            if (ball.velocity.y < 0) ball.velocity.y *= -1;
        } else if (ball.position.y + ball.radius >= maxY) {
            ball.position.y = maxY - ball.radius - 1;
            if (ball.velocity.y > 0) ball.velocity.y *= -1;
        }

		Cell* newCell = grid->getCell(ball.position);
		if (newCell != ball.ownerCell) {
			grid->removeBallFromCell(&balls[i]);
			grid->addBall(&balls[i], newCell);
		}
    }

	// Check collisions
	/*for (size_t j = i + 1; j < balls.size(); j++) {
		checkCollision(ball, balls[j]);
	}*/
	updateCollision(grid);

    // Update our grabbed ball
    if (m_grabbedBall != -1) {
        // Update the velocity again, in case it got changed by collision
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
        m_prevPos = balls[m_grabbedBall].position;
    }
}

void BallController::onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY) {
    for (size_t i = 0; i < balls.size(); i++) {
        // Check if the mouse is hovering over a ball
        if (isMouseOnBall(balls[i], mouseX, mouseY)) {
            m_grabbedBall = i; // BE AWARE, if you change the order of the balls in the vector, this becomes invalid!
            m_grabOffset = glm::vec2(mouseX, mouseY) - balls[i].position;
            m_prevPos = balls[i].position;
            balls[i].velocity = glm::vec2(0.0f);
        }
    }
}

void BallController::onMouseUp(std::vector <Ball>& balls) {
    if (m_grabbedBall != -1) {
        // Throw the ball!
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
        m_grabbedBall = -1;
    }
}

void BallController::onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY) const {
    if (m_grabbedBall != -1) {
        balls[m_grabbedBall].position = glm::vec2(mouseX, mouseY) - m_grabOffset;
    }
}

void BallController::checkCollision(Ball& b1, Ball& b2) const {
    // We add radius since position is the top left corner
    glm::vec2 distVec = b2.position - b1.position;
    glm::vec2 distDir = glm::normalize(distVec);
    float dist = glm::length(distVec);
    float totalRadius = b1.radius + b2.radius;
    
    float collisionDepth = totalRadius - dist;
    // Check for collision
    if (collisionDepth > 0) {

        // Push away the balls based on ratio of masses
		b1.position -= distDir * collisionDepth * (b2.mass / b1.mass)*0.5f;
		b2.position += distDir * collisionDepth * (b1.mass / b2.mass)*0.5f;

        // Calculate deflection. http://stackoverflow.com/a/345863
        /*float aci = glm::dot(b1.velocity, distDir) / b2.mass;
        float bci = glm::dot(b2.velocity, distDir) / b1.mass;

        float massRatio = b1.mass / b2.mass;

        b1.velocity += (bci - aci) * distDir * (1.0f / massRatio);
        b2.velocity += (aci - bci) * distDir * massRatio;*/

	    auto aci = glm::dot(b1.velocity, distDir);
	    auto bci = glm::dot(b2.velocity, distDir);

	    auto acf = (aci * (b1.mass - b2.mass) + 2 * b2.mass * bci) / (b1.mass + b2.mass);
	    auto bcf = (bci * (b2.mass - b1.mass) + 2 * b1.mass * aci) / (b1.mass + b2.mass);

		b1.velocity += (acf - aci) * distDir;
		b2.velocity += (bcf - bci) * distDir;

		if(glm::length(b1.velocity + b2.velocity) > 0.5f) {
			auto choice = glm::length(b1.velocity) < glm::length(b2.velocity);
			choice ? b2.color = b1.color : b1.color = b2.color;
		}
    }
}

void BallController::checkCollision(Ball * ball, std::vector<Ball*>& ballsToCheck, int startingIndex) const {
	for (size_t i = startingIndex; i < ballsToCheck.size(); i++) {
		checkCollision(*ball, *ballsToCheck[i]);
	}
}

void BallController::updateCollision(Grid * grid) const {
	for (size_t i = 0; i < grid->m_cells.size(); i++) {
		auto x{ i % grid->m_numXCells };
		auto y{ i / grid->m_numXCells };

		Cell& cell{ grid->m_cells[i] };
		for (size_t j = 0; j < cell.balls.size(); j++) {
			Ball* ball = cell.balls[j];
			checkCollision(ball, cell.balls, j+1);//update with residing cell
			//update collision with neighbor cells
			if (x > 0) {
				checkCollision(ball, grid->getCell(x - 1, y)->balls, 0); //left
				if (y > 0) checkCollision(ball, grid->getCell(x - 1, y - 1)->balls, 0); //top left
				if (y < static_cast<unsigned>(grid->m_numYCells-1)) checkCollision(ball, grid->getCell(x - 1, y + 1)->balls, 0); //bottom left
			}
			if (y > 0) {
				if (y > 0) checkCollision(ball, grid->getCell(x, y - 1)->balls, 0); //top left
			}
		}
	}
}

bool BallController::isMouseOnBall(Ball&b, float mouseX, float mouseY) const {
    return (mouseX >= b.position.x - b.radius && mouseX < b.position.x + b.radius * 2.0f &&
            mouseY >= b.position.y - b.radius && mouseY < b.position.y + b.radius * 2.0f);
}

glm::vec2 BallController::getGravityAccel() const {
    const float GRAVITY_FORCE = 0.1f;
    glm::vec2 gravity;

    switch (m_gravityDirection) {
        case GravityDirection::DOWN:
            gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
            break;
        case GravityDirection::LEFT:
            gravity = glm::vec2(-GRAVITY_FORCE, 0.0f);
            break;
        case GravityDirection::RIGHT:
            gravity = glm::vec2(GRAVITY_FORCE, 0.0f);
            break;
        case GravityDirection::UP:
            gravity = glm::vec2(0.0f, GRAVITY_FORCE);
            break;
        default:
            gravity = glm::vec2(0.0f);
            break;
    }
    return gravity;
}
