
import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.ExecutionException;

public class FileUpload {


   public static void main(String[] args) {

      String uploadServerUriFile = "http://14.63.196.110/api/user_profile_upload_file.php";
       int serverResponseCode = 0;
      
      String fileName = args[0];
      String id = args[1];
      String res;
      String uploadServerUriFiles = "http://14.63.196.110/api/set_timeline.php";
      //user_id=11image=abcd.png;
      String[] array;
      array = fileName.split("/");
      
      String filename1 = "image="+array[array.length-1]+"&"+id;
      System.out.println(filename1);
      HttpURLConnection conns = null;

            
      HttpURLConnection conn = null;
      DataOutputStream dos = null;
      String lineEnd = "\r\n";
      String twoHyphens = "--";
      String boundary = "*****";
      int bytesRead, bytesAvailable, bufferSize;
      byte[] buffer;
      int maxBufferSize = 1 * 1024 * 1024;
      File sourceFile = new File(fileName);

      if (!sourceFile.isFile()) {
         
         return;
      } else {
         try {
                // open a URL connection to the Servlet
                FileInputStream fileInputStream = new FileInputStream(sourceFile);
                URL url = new URL(uploadServerUriFile);
                // Open a HTTP  connection to  the URL
                conn = (HttpURLConnection) url.openConnection();
                conn.setDoInput(true); // Allow Inputs
                conn.setDoOutput(true); // Allow Outputs
                conn.setUseCaches(false); // Don't use a Cached Copy
                conn.setRequestMethod("POST");
                conn.setRequestProperty("Connection", "Keep-Alive");
                conn.setRequestProperty("ENCTYPE", "multipart/form-data");
                conn.setRequestProperty("Content-Type", "multipart/form-data;boundary=" + boundary);
                conn.setRequestProperty("uploaded_file", fileName);

                dos = new DataOutputStream(conn.getOutputStream());

                dos.writeBytes(twoHyphens + boundary + lineEnd);
                dos.writeBytes("Content-Disposition: form-data; name=\"uploaded_file\";filename=\""
                        + fileName + "\"" + lineEnd);
                dos.writeBytes(lineEnd);

                // create a buffer of  maximum size
                bytesAvailable = fileInputStream.available();

                bufferSize = Math.min(bytesAvailable, maxBufferSize);
                buffer = new byte[bufferSize];

                // read file and write it into form...
                bytesRead = fileInputStream.read(buffer, 0, bufferSize);

                while (bytesRead > 0) {

                    dos.write(buffer, 0, bufferSize);
                    bytesAvailable = fileInputStream.available();
                    bufferSize = Math.min(bytesAvailable, maxBufferSize);
                    bytesRead = fileInputStream.read(buffer, 0, bufferSize);

                }

                // send multipart form data necesssary after file data...
                dos.writeBytes(lineEnd);
                dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);

                // Responses from the server (code and message)
                serverResponseCode = conn.getResponseCode();
                String serverResponseMessage = conn.getResponseMessage();


                if(serverResponseCode == 200){
                System.out.println("FileUploadedok");
                }

                //close the streams //
                fileInputStream.close();
                dos.flush();
                dos.close();
                // open a URL connection to the Servlet
                URL urls = new URL(uploadServerUriFiles);
                // Open a HTTP  connection to  the URL
                conns = (HttpURLConnection) urls.openConnection();
                conns.setDoInput(true); // Allow Inputs
                conns.setDoOutput(true); // Allow Outputs
                conns.setUseCaches(false); // Don't use a Cached Copy
                conns.setRequestMethod("POST");
                conns.setRequestProperty("Accept", "application/json");
                //웹서버 형식으로 전송
                conns.setRequestProperty("content-type", "application/x-www-form-urlencoded");

                System.out.println(filename1);
                byte[] outputInBytes = filename1.getBytes("UTF-8");

                
                OutputStream os = conns.getOutputStream();
                os.write(outputInBytes);
                os.flush();
                os.close();
              //결과가 참인지 알려주는 부분.
                int retCode = conns.getResponseCode();
                if(retCode == HttpURLConnection.HTTP_OK) {
                    InputStream is = conns.getInputStream();
                    BufferedReader br = new BufferedReader(new InputStreamReader(is));
                    String line;
                    StringBuffer response = new StringBuffer();
                    while ((line = br.readLine()) != null) {
                        response.append(line);
                    }
                    res = response.toString();
                    System.out.println(res);

                    br.close();
                    is.close();
                }
                
            } catch (MalformedURLException ex) {

                ex.printStackTrace();

            } catch (Exception e) {

                e.printStackTrace();

            }
            return;

      }
      
   }

}