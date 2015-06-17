<form action="<?php echo $_SERVER['REDIRECT_URL']; ?>?destination=<?php echo urlencode($_SERVER['REDIRECT_URL']); ?>"  accept-charset="UTF-8" method="post" id="user-login-form">
  <div>ss
    <div class="form-item" id="edit-name-wrapper">
      <label for="edit-name">Username</label>
      <input type="text" maxlength="60" name="name" id="edit-name" size="15" value="" class="form-text required" />
    </div>
    <div class="form-item" id="edit-pass-wrapper">
      <label for="edit-pass">Password</label>
      <input type="password" name="pass" id="edit-pass"  maxlength="60"  size="15"  class="form-text required" />
    </div>
    <a href="/user/password" title="Request new password via e-mail." class="forgot-pass">Forgot Password?</a><input type="submit" name="op" id="edit-submit" value="Log in"  class="form-submit" />
    <input type="hidden" name="form_build_id" id="<?php form_clean_id('edit-'. drupal_get_token()  .'-form-token') ?>" value="<?php drupal_get_token() ?>"  />
    <input type="hidden" name="form_id" id="edit-user-login-block" value="user_login_block"  />
  </div>
</form>