#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include  "Representations/Perception/GoalPercept.h"

int  IfKickRight(const ObstacleModel& theObstacle,  const GoalPercept& goalpost  );