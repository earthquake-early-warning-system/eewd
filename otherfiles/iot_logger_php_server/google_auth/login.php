<?php

// Include configuration file
require_once 'config.php';


if(isset($_GET['code'])){
    $client->authenticate($_GET['code']);
    $_SESSION['token'] = $client->getAccessToken();
    header('Location: ' . $redirectUri);
    echo 'code ';
}

if(isset($_SESSION['token'])){
    $client->setAccessToken($_SESSION['token']);
    echo 'token ';
}

/* Refresh token when expired */
// if ($client->isAccessTokenExpired()) {
//     // the new access token comes with a refresh token as well
//     $client->fetchAccessTokenWithRefreshToken($client->getRefreshToken());
//     file_put_contents($TOKEN_FILE, json_encode($client->getAccessToken()));
//     echo 'refresh ';
// }


if($client->getAccessToken()){
    // Get user profile data from google
    //$google_oauthV2 = new Google_Service_Oauth2($client);
    $gpUserProfile = $google_oauthV2->userinfo->get();
    
    // Initialize User class
    //$user = new User();
    
    // Getting user profile info
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
    //$userData = $user->checkUser($gpUserData);
    
    // Storing user data in the session
    $_SESSION['userData'] = $gpUserData;

    print_r($gpUserData);
    
    // Render user profile data
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
        $output .= '<p><a href="'.$gpUserData['link'].'" target="_blank">Click to visit Google+</a></p>';
        $output .= '<p>Logout from <a href="logout.php">Google</a></p>';
        $output .= '</div>';
    }else{
        $output = '<h3 style="color:red">Some problem occurred, please try again.</h3>';
    }
}else{
    // Get login url
    $authUrl = $client->createAuthUrl();
    
    // Render google login button
    $output = '<a href="'.filter_var($authUrl, FILTER_SANITIZE_URL).'"><img src="btn_google_signin_light_focus_web@2x.png" alt="click"/></a>';
}
?>

<p align="center">
<!--div class="container"-->
    <!-- Display login button / Google profile information -->
    <?php echo $output; ?>
<!--/div-->
</p>
