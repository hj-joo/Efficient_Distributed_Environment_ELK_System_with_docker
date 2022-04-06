#include <stdio.h>
#include <time.h>

int main(void) {

   //clock_t start, end;
  // float time_interval;

   char* text = "1.2.3.4 - - [28/Apr/2019:00:01:58 +0900] \"GET /assets/css/reset.css HTTP/1.1\" 200 1075 \"https://www.example.com/login/form.do\" \"Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\"\n";
   FILE* fp = fopen("y.log", "a");

   
  // start = clock();
   

   
   /*10만개
       for(int j=0; j<60; j++)
   { 
      for (int i = 0; i < 100000; i++) {
         fprintf(fp, "%s", text);
         //Sleep(1);   
         }
      Sleep(650);
      }
   */


   /*
   //5만개
   for (int j = 0; j < 60; j++)
   {
      for (int i = 0; i < 50000; i++) {
         fprintf(fp, "%s", text);
         //Sleep(1);   
      }
      Sleep(830);
   }
   */

   
   /*
   //1만개   
   for (int j = 0; j < 60; j++)
   {
      for (int i = 0; i < 10000; i++) {
         fprintf(fp, "%s", text);
         //Sleep(1);   
      }
      Sleep(960);
   }
   */
   //한번에 백만게

      for (int i = 0; i < 300000; i++) {
         fprintf(fp, "%s", text);
         //Sleep(1);   
      }
  // end = clock();
  // time_interval = (float)(end - start) / CLOCKS_PER_SEC;

   fclose(fp);

//   printf("파일 열고 쓰는 시간- 파일 닫는 시간=%.3f", time_interval);

   return 0;
}
