
option(FocusBall, (float) (0.38f) tilt)
{
  /** Simply sets the necessary angles */
  initial_state(lookForward)
  {
    action
    {
    	float pan = theBallModel.estimate.position.angle();//
    	SetHeadPanTilt( pan , tilt, 300_deg);
    }
  }
}
