# REve
Eve-7 based event display for Mu2e. Eve-7 (formally REve) is an updated version of the well-known and widely used TEve ROOT based Event Visualization Software. The purpose is to update the TEve code, REve uses modernized infrastcutrue and allows Web Access for remote use.

For more information about Eve-7 and the implementation for Mu2e see: https://mu2ewiki.fnal.gov/wiki/Eve7EventDisplay#Examples_of_the_Eve-7_Mu2e_Display.

# Run an Example

You can run the REve Mu2e code in the same way as any analyzer in Mu2e Offline. For example on the reconstructed outcome of a CeEndpoint (made using the Production/JobConfig fcl files):

```
mu2e -c REve/fcl/RunDisplay.fcl FILENAME.art
```

A firefox window will open and you should see your events displayed.

# Development

This code is still under development. It has been written so far by Sophie Middleton (Caltech), please contact to contribute.
