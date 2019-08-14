/** Pun intended. Looks straight ahead in a way that the Nao V4's cameras cover the area in front of its feet as well as everything else in front of the robot.*/
option(LookUp, (float) (0.f) tilt)
{
  /** Simply sets the necessary angles */
  initial_state(lookLeft)
  {
  	transition{
  ;
  	}
    action
    {
      SetHeadPanTilt(0.f, tilt, 50_deg);
    }
  }

}
