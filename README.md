# REve
Eve-7 based event display for Mu2e. Eve-7 (formally REve) is an updated version of the well-known and widely used TEve ROOT based Event Visualization Software. The purpose is to update the TEve code, REve uses modernized infrastcutrue and allows Web Access for remote use.

For more information about Eve-7 and the implementation for Mu2e see: https://mu2ewiki.fnal.gov/wiki/Eve7EventDisplay#Examples_of_the_Eve-7_Mu2e_Display.

# Run an Example

You can run the REve Mu2e code in the same way as any analyzer in Mu2e Offline. 

A number of "examples" are contained in the "examples" directory. For example on the reconstructed outcome of a CeEndpoint (made using the Production/JobConfig fcl files):

```
mu2e -c REve/fcl/helix_example.fcl FILENAME.art
```
Things can be added and removed on the GUI once set to "true" in the FCL.

# The Browser

The user is advised to add the following to their .rootrc:

```
WebEve.DisableShow:   1
WebGui.HttpPort:   1234
```
The first line disables the forefox window pop up on the Mu2e machines. The second line sets port number to 1234, this can then be used in your local machine. Assuming you have access to the Fermilab network (either on site or using VPN):

```
mu2egpvm0X.fnal.gov:1234/win1/

```

Or create na ssh tunnel:
```
ssh -KXY -L 1234:mu2egpvm02.fnal.gov:1234 user@mu2egpvm02.fnal.gov

```

# Development

This code is still under development. It has been written so far by Sophie Middleton (Caltech), please contact to contribute.
