//currently only able to convert values between 0 and 34 decrees/C
//Takes in voltage reading from sensor in millivolts and outputs a value in decrees/C

int voltsToTemp(int MV)
{
  //array that stores voltage values which correspond to temperatures between 0 and 34 decrees C
  int temperatures[] = {4366, 4347, 4331, 4316, 4297, 4279, 4262, 4242, 4203, 4182, 4162, 4146, 4129, 4109, 4088, 4062, 4022, 4001, 3979, 3955, 3932, 3907, 3880, 3858, 3756, 3721, 3682, 3641, 3600, 3557, 3516, 3482, 3452, 3423, 3391};
  int i = 0, j = 34, place =0;
  //
  int distance = 5001;
  
  while(i < j)
  {
    //check if the distance from current value is smaller than the previous
    if(abs(MV-temperatures[i]) <= distance)
    {
      //Store new value for distance and place
      distance = abs(MV - temperatures[i]);
      place = i;
    }
    i++;
  }
  return place;
}
