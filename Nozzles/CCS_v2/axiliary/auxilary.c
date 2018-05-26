void update_up(void)
{
nozzle_off();
   if (BUTT_PRESSED(UP_BUTT))
   {
      LED_ON(GREEN_LED);
      if(rpm < MAX_RPM)
      {
         rpm += RPM_STEP;
         message = RPM_PRINT;      //THIS IS THE LIN I GOT THE ERROR ! ! ! 
         to_print = true;
      }
      else 
      {
         message = MAX_VAL_REACHED;
         to_print = true;
      }
   }
   if (BUTT_PRESSED(DOWN_BUTT))
   {
      LED_ON(RED_LED);
      if (rpm > MIN_RPM)
      {
         rpm -= RPM_STEP;
         message = RPM_PRINT;
         to_print = true;
      }
      else 
      {
         message = MIN_VAL_REACHED;
         to_print = true;
      }
      
   }
}
//LEDS TO BE SET OFF, TO_PRINT TO BE SET FALSE,
