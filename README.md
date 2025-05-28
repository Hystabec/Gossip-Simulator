# Gossip-Simulator

Only `Windows` is supported.  

## Simulation Description
The simulation start as soon as the `gossipSim` program is executed.  
The number of NPCs and their relationships is defined in `NPC_Data.xml`.  

Each `NPC Tick` or `2 seconds` the NPCs will be updated.  
Each `NPC Tick` if a gossip instance is to start on that tick (specified in `Gossip_Data.xml`), the gossip will be given to the NPC and then they will start spreading it `next tick`.  

When an NPC hears gossip the will wait `1 tick` before spreading it.  

IF the NPC has heared a gossip, but is yet to tell it, they will attempt tell the NPCs they know:  
if the gossip is positive AND about an NPC they like, they will tell all the other NPCs that they like  
if the gossip is negative AND about an NPC they dislike, they will tell all the other NPCs that they like  

When hearing gossip the NPC will choose to `Remember` or `ignore` it.  

NPCs with `no specified personality` will follow these rules:  
NPCs will always ignore gossip about themselves.  
NPCs will also ignore gossip about NPCs they don't know.  

This table shows the outcome of gossip type when about known NPCs:  
| About NPC relation | Gossip type | Outcome  |  
| ------------------ | ----------- | -------- |  
| Positive           | Positive    | Remember |  
| Positive           | Negative    | Ignore   |  
| Negative           | Positive    | Ignore   |  
| Negative           | Negative    | Remember |  

NPCs can have a `personality` which can be either of the following: `Spreader` or `Sink`.  
NPCs with the `Spreader` personality will listen to and spread all gossip as long as the gossip is not about them.  
NPCs with the `Sink` personality will listen to all gossip but will never spread gossip.  


## How to use
`gossipSim` is the primary program that allows the simulation to be viewed.  
When opened a `window` and `console` will spawn.  
The `console` will show information about: when a new gossip insance is started, when an NPC start telling gossip, When an NPC tells another NPC gossip and the outcome of the gossip telling (will be remembered or ignored) with the reason for the outcome.  
The `window` shows all the NPCs (white circles as default) as well as 2 pannels `NPC Details` and `Gossip Selector`.  
When a `NPC is hovered`, with the mouse cursor, the NPCs will change colour to reflect their relationships with the other NPC `Green is liked` and `Red is disliked`,  
while the NPC remains hovered, the `NPC Details` pannel will display the relationships of the hovered NPC.  
The `NPC Details` pannel also shows the `NPCs ticked counter` this counter is updated each time the NPCs are ticked.  
The `Gossip Selector` pannel contains a list of the currently active gossip instancese, selecting a gossip from the list will cause the NPCs that have heard the gossip to `highlight in purple`.  

The NPCs are loaded from `NPC_Data.xml` when the program is started an will be renderer to the screen.  
The gossip instances are loaded from `Gossip_Data.xml` and will occure on the `startTick` specified in the file, the start tick relates to the `NPCs ticked counter` on the `NPC Details` pannel.  
NPCs will wait `1 NPC tick` after hearing a gossip instance before the start spreading it.  

-----

`gossipSim-nodeEditor` is a helper program that allow the viewing of the contents of the XML data files.  
Currently the program only has the ability to read the files.  
When executed the user will need to open an XML file `File -> Open`.  
Select either `NPC_Data.xml` OR `Gossip_Data.xml`.  
Then select an NPC from the `NPC List` pannel the view the details about the NPC.  
If the gossip file was opened, select a gossip instance from the `Gossip List` pannel the view the details about the gossip.  


## Editing XML files
Currently `gossipSim` only has the ability to read `NPC_Data.xml` and `Gossip_Data.xml`.  
So to effect the simulation these files will need to be changed/updated.  

NPC data can be Edited in the `NPC_Data.xml` file.  
The NPC data file follows the structure.  
```
<listofNPC>
    <NPC name="(NPC NAME)">
        <relationships>
            <relation npc="(OTHER NPC)" value="(INT VALUE)" />
        </relationships>
    </NPC>
</listOfNPC>
```
Example:  
```xml
<listofNPC>
    <NPC name="1">
        <relationships>
            <relation npc="2" value="10" />
        </relationships>
    </NPC>
    <NPC name="2">
        <relationships>
            <relation npc="1" value="10" />
        </relationships>
    </NPC>
</listOfNPC>
```
NPCs can have a specified personality `spreader` or `sink`, if no personality is specifed the will follow the default rules.  
Currently any value `greater than 0` is classed as a `positive` relation.  
Any value `less than 0` is classed as a `negative` relation.  

---

Gossip events can be Edited in the `Gossip_Data.xml` file.  
The Gossip data file follows the structure.  
```
<listOfGossipEvents>
	<Event id="(CUSTOM ID)" type="('positive' OR 'negative')" about="(NPC NAME)" startingFrom="(NPC NAME)" startTick="(THE NPC TICK TO START ON)" />
</listOfGossipEvents>
```
Example:  
```xml
<listOfGossipEvents>
	<Event id="1" type="positive" about="Bravo" startingFrom="Alpha" startTick="0" />
	<Event id="2" type="negative" about="Alpha" startingFrom="Delta" startTick="10" />
</listOfGossipEvents>
```


## Installation

### Install with Git
```bash
git clone https://github.com/Hystabec/Gossip-Simulator
cd Gossip-Simulator
git submodule update --init --recursive
```

### Install manually
Download project files.  
Download Submodules:
- submodule URLs and Paths can be found in `.gitmodules`

### Building project files for Visual Studio 2022
Once the repository has been cloned.  
Run `GenerateProject_VS22.bat` to generate visual studio 2022 project files.  
Open in visual studio 2022 then Build project.  
The project can then be ran.