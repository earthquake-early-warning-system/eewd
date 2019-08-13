<?php
require_once 'vendor/autoload.php';
 
// init configuration
$clientID = '431490386846-mj5e7v3icscn2n0gr4vfpdvnv93gpb6h.apps.googleusercontent.com';
$clientSecret = '3vqqIfmtLah_UYNa7w4Q6oKm';
$redirectUri = 'http://' . $_SERVER['HTTP_HOST'] . '/redirect.php';
   
// create Client Request to access Google API
$client = new Google_Client();
$client->setClientId($clientID);
$client->setClientSecret($clientSecret);
$client->setRedirectUri($redirectUri);
$client->setApplicationName('EEWD');
$client->setApprovalPrompt('force');
$client->addScope("email");
$client->addScope("profile");

//$google_oauthV2 = new Google_Service_Oauth2($client); 
?>
