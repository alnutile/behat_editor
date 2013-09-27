# SauceLabs and Behat

This will call to the Saucelabs API to run the test.
A new button will appear if you have permissions on your View, Edit and Add pages

see install.txt for more info.


# Todo

See if there is a way to name the test other than the behat.yml file.
Or some other way to know what test we just ran since the command line

```bash
./bin/behat -p Selenium-saucelabs2 *.features
```

Does not let me know the ID of the test so this module just gets the last job id before running the test
and then the newest one made while the tests runs.
But if two people make a test at the same time there could be a collision of who's test it is.
