#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main (int argc, char* argv[])
{

   // Initialize Connection
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   if (!(conn = mysql_init(0)))
   {
      fprintf(stderr, "unable to initialize connection struct\n");
      exit(1);
   }

   // Connect to the database
   if (!mysql_real_connect(
         conn,                 // Connection
         "localhost",// Host
         "Daytoo",            // User account
         "Magnfacnt123",   // User password
         "sensor_data",               // Default database
         3306,                 // Port number
         NULL,                 // Path to socket file
         0                     // Additional options
      ))
   {
      // Report the failed-connection error & close the handle
      fprintf(stderr, "Error connecting to Server: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }

    if (mysql_query(conn, "show tables")) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
    }

    res = mysql_use_result(conn);

    printf("MySQL Tables in mysql database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
          printf("%s \n", row[0]);

 

   // Close the Connection
    mysql_free_result(res);
    mysql_close(conn);

   return 0;
}
