<?php
// Include configuration file
require_once 'config.php';
 
// authenticate code from Google OAuth Flow
if (isset($_GET['code'])) {
  $token = $client->fetchAccessTokenWithAuthCode($_GET['code']);
  $client->setAccessToken($token['access_token']);
  
  // get profile info
  $google_oauth = new Google_Service_Oauth2($client);
  $gpUserProfile = $google_oauth->userinfo->get();
  $gpUserData = array();
    $gpUserData['oauth_uid']  = !empty($gpUserProfile['id'])?$gpUserProfile['id']:'';
    $gpUserData['first_name'] = !empty($gpUserProfile['given_name'])?$gpUserProfile['given_name']:'';
    $gpUserData['last_name']  = !empty($gpUserProfile['family_name'])?$gpUserProfile['family_name']:'';
    $gpUserData['email']      = !empty($gpUserProfile['email'])?$gpUserProfile['email']:'';
    $gpUserData['gender']     = !empty($gpUserProfile['gender'])?$gpUserProfile['gender']:'';
    $gpUserData['locale']     = !empty($gpUserProfile['locale'])?$gpUserProfile['locale']:'';
    $gpUserData['picture']    = !empty($gpUserProfile['picture'])?$gpUserProfile['picture']:'';
    $gpUserData['link']       = !empty($gpUserProfile['link'])?$gpUserProfile['link']:'';
    
    // Insert or update user data to the database
    $gpUserData['oauth_provider'] = 'google';


    if(!empty($gpUserData)){
      $output  = '<h2>Google Account Details</h2>';
      $output .= '<div class="ac-data">';
      $output .= '<img src="'.$gpUserData['picture'].'">';
      $output .= '<p><b>Google ID:</b> '.$gpUserData['oauth_uid'].'</p>';
      $output .= '<p><b>Name:</b> '.$gpUserData['first_name'].' '.$gpUserData['last_name'].'</p>';
      $output .= '<p><b>Email:</b> '.$gpUserData['email'].'</p>';
      $output .= '<p><b>Gender:</b> '.$gpUserData['gender'].'</p>';
      $output .= '<p><b>Locale:</b> '.$gpUserData['locale'].'</p>';
      $output .= '<p><b>Logged in with:</b> Google</p>';
      //$output .= '<p><a href="'.$gpUserData['link'].'" target="_blank">Click to visit Google+</a></p>';
      //$output .= '<p>Logout from <a href="logout.php">Google</a></p>';
      $output .= '</div>';

      include_once '../device_reg.php'; // Under test and dev
  }else{
      $output = '<h3 style="color:red">Some problem occurred, please try again.</h3>';
  }
    
     
  // now you can use this profile info to create account in your website and make user logged in.
} else {
  $authUrl = $client->createAuthUrl();
  $output = '<a href="'.filter_var($authUrl, FILTER_SANITIZE_URL).'"><img src="btn_google_signin_light_focus_web@2x.png" alt="click"/></a>';

  //echo $output;//"<a href='".$client->createAuthUrl()."'>Google Login</a>";
}
?>

<p align="center">
<!--div class="container"-->
    <!-- Display login button / Google profile information -->
    <?php echo $output; ?>
<!--/div-->
</p>
