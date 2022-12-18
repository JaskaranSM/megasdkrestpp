# MEGASDK-REST (PP)

This is a cpp based microservice powered by MegaSDK, served and SimpleJSON. This microservice is designed to sit in a docker container and handle the downloading of data from mega.nz file sharing service to the local filesystem. It is acheived using the downloading functionalities of MegaSDK. The microservice exposes certain endpoints via a webserver powered by served that can be used to control the microservice. The webserver expects the clients to send payloads as JSON.

## Endpoints

- `POST /login`  
  This endpoint is used to perform a login request on the MegaApi object so that the further requests are authenticated such as transfers.  
  Request payload:
  ```
  {
    "email": "mega_email",
    "password: "password"
  }
  ```
  Response payload:
  ```
  {
    "error_code: 0,
    "error_string: ""
  }
  ```
- `POST /adddownload`  
  This endpoint is used to create a download from the mega.nz shareable link in the provided directory in the request payload. This method returns a random value called GID which will be used to reference this download in other requests. The clients are supposed to save it somewhere.  
  Request payload:
  ```
  {
    "link":"https://mega.nz/folder_or_file_link",
    "dir":"directory to download the data in on the local system"
  }
  ```
  Response payload:
  ```
  {
    "gid": "random_string",
    "error_code: 0,
    "error_string: ""
  }
  ```
- `POST /getstatus`  
  This endpoint provides the current status of the download registered in the download manager by using the gid provided in request payload.  
  Request payload:
  ```
  {
    "gid": "random_string_returned_by_adddownload"
  }
  ```
  Response payload:
  ```
  {
    "error_code": 0,
    "error_string": "",
    "name": "name of the root node of mega.nz link",
    "completed_length": 0,
    "total_length": 0,
    "speed": 0,
    "state": state_of_transfer_int,
    "is_completed": false,
    "is_failed": false,
    "is_cancelled": false
  }
  ```
- `POST /canceldownload`  
  This endpoint is for cancelling download present in the download manager. The gid is used to reference the download to cancel.  
  Request payload:
  ```
  {
    "gid": "random_string_returned_by_adddownload"
  }
  ```
  Response payload:
  ```
  {
    "error_code: 0,
    "error_string: ""
  }
  ```

## Errors
  The error codes are a merge from MegaSDK and custom ones to easy to handle in the client, such as 0 is used to indicate there is no error as same as MegaSDK library. Along with error codes the API server also provides error strings if possible, and they can be used by the clients to show what went wrong.

## Configuration
  This microservice is intended be used in a docker compose or container based system so configuration is handled by environment variables.  
  List of environment variables available:
  ```
  MEGA_API_KEY= mega developer key for megasdk
  APP_PORT= port for webserver to bind to, defaults to 8069
  LOG_FILE= log file to write logs into, defaults to log.txt
  APP_THREADS= threads to be used by served's webserver, defaults to 1
  MEGA_THREADS= threads to be used by MegaSDK's worker manager, defaults to 1
  MEGA_DEBUG= true for setting MegaSDK's log level to debug, defaults to false
  ```
  Only `MEGA_API_KEY` is required to be provided by user to have a working microservice server. However it is recommended to set the other options as well.

## Building and running
  Docker build is the most straightforward and easy method to build. The provided script auto detects and provides patches for linux-arm64 architecture
  - clone the repository with submodules recursion enabled.
  ```
  git clone --recurse-submodules https://github.com/JaskaranSM/megasdkrestpp
  ```
  - run docker build 
  ```
  docker build . -t megasdkrestpp
  ```
  - run the container
  ```
  docker run -e MEGA_API_KEY=your_key megasdkrestpp
  ```

## Motivation
  The MegaSDK c++ library is very well built but being written in c++, It does provide bindings to other languages but they are only for some languages and not very straightforward to use (imo). So to accumulate download from mega.nz functionality in one of my Go projects, This microservice was built so that the client app can communicate to it with a shared docker network.  

 
