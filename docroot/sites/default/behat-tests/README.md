# Behat Tests

This directory contains [Behat](http://behat.org) tests for the warmshowers.org website. The tests are located in the `features` sub-directory.

## Quickstart

You'll need to run the following steps.

```
cd docroot/sites/default/behat-tests
curl http://getcomposer.org/installer | php
php composer.phar install
```

### Selenium

Some tests require Javascript (look for the @javascript tag above the scenario definition). In that case, you will need to [install Selenium](https://github.com/facebook/php-webdriver/blob/master/README.md); the short version is that you will download a [selenium-server-standalone-#.jar](https://code.google.com/p/selenium/downloads/list) file and then run `java -jar selenium-server-standalone-#.jar` where `#` is the version of the file you've downloaded.

Once you have downloaded the file, you can create an alias in your `.bashrc` file, for example: `alias selenium="java -jar ~/src/selenium/selenium-server-standalone-2.25.0.jar"`. Then you can type `selenium` in your terminal and you will be up and running.

## Set up

- All tests are written for a local development environment.

- Your local development site's URL must be: http://warmshowers.dev

- It is important that you use a development environment rebuilt using Drush Rebuild because this rebuild script will enable the `reroute_email` module. **If this module is not enabled, the tests may generate unwanted e-mails.**

- Make sure that `reroute_email` is enabled *and* properly configured with the an e-mail address to reroute mail to.

### Drush aliases

You must have a Drush alias for working with your local environment *and* your local drush alias must be called `@warmshowers.dev` for the tests to run correctly.

## Usage

First, navigate to the `behat-tests` directory: `cd docroot/sites/default/behat-tests`

To run all tests: `bin/behat`

To run a specific test: `bin/behat features/{name}.feature`
