# Gossip-Simulator

Only `Windows` is supported.  
Compiled program can be found on the [Release Tab](https://github.com/Hystabec/Gossip-Simulator/releases)  

## Simulation Description
The simulation start as soon as the `gossipSim` program is executed.  
The number of NPCs and their relationships is defined in `NPC_Data.xml`.  

Each `NPC Tick` (`2 seconds`) the NPCs will be updated.  
Each `NPC Tick` if a gossip instance is to start on that tick (specified in `Gossip_Data.xml`), the gossip will be given to the NPC and then they will start spreading it `next tick`.  
When an NPC hears gossip the will wait `1 tick` before spreading it.  

Gossip has 3 types:  
1. `Positive`
2. `Negative`
3. `Neutral`

NPCs can either have a `Positve` or `Negative` relationship with other NPCs.  

IF the NPC has heared a gossip, but is yet to tell it, they will attempt tell the NPCs they know:  
NPCs will tell gossip to everyone that they know AND have a positive relationship with.  

When hearing gossip the NPC will choose to `Remember` or `ignore` it.  
NPCs will always ignore gossip they have already heard.  

NPCs with `no specified personality` will follow these rules:  
NPCs will always ignore gossip about themselves.  
NPCs will also ignore gossip about NPCs they don't know.  
The table below shows the outcome of the gossip, when about known NPCs:  
| About NPC relation is | Gossip type is | Outcome  |  
| --------------------- | -------------- | -------- |  
| Positive              | Positive       | Remember |  
| Positive              | Negative       | Ignore   |  
| Positive              | Neutral        | Remember |  
| Negative              | Positive       | Ignore   |  
| Negative              | Negative       | Remember |  
| Negative              | Neutral        | Ignore   |  

NPCs can have a `personality` which can be either of the following: `Spreader` or `Sink`.  
NPCs with the `Spreader` personality will listen to and spread all gossip as long as the gossip is not about them.  
NPCs with the `Sink` personality will listen to all gossip but will never spread gossip.  


## How to use
`gossipSim` is the primary program that allows the simulation to be viewed.  
When opened a `window` and `console` will spawn.  

The `console` will show information about: 
1. when a new gossip insance is started
2. when an NPC start telling gossip
3. When an NPC tells another NPC gossip and the outcome of the gossip telling (remembered or ignored), with the reason for the outcome.  

The `window` shows all the NPCs (white circles as default) as well as 2 pannels `NPC Details` and `Gossip Selector`.  
When a `NPC is hovered`, with the mouse cursor, the `other NPCs` will `change colour` to reflect their relationships with the other NPC `Green is liked` and `Red is disliked`,  
while the NPC remains hovered, the `NPC Details` pannel will display the relationships of the hovered NPC.  
The `NPC Details` pannel also shows the `NPCs ticked counter` this counter is updated each time the NPCs are ticked.  
The `Gossip Selector` pannel contains a list of the currently active gossip instancese, selecting a gossip from the list will cause the NPCs that have heard the gossip to `highlight in purple`.  

The NPCs are loaded from `NPC_Data.xml` when the program is started an will be renderer to the screen.  
The gossip instances are loaded from `Gossip_Data.xml` and will occure on the `startTick` specified in the file, the start tick relates to the `NPCs ticked counter` on the `NPC Details` pannel.  
NPCs will wait `1 NPC tick` after hearing a gossip instance before the start spreading it.  

-----

`gossipSim-nodeEditor` is a helper program that allow the viewing of the contents of the XML data files with a UI.  
(Currently the program only has the ability to read the files.)  
When executed the user will need to open an XML file `File -> Open`.  
Select either `NPC_Data.xml` OR `Gossip_Data.xml`.  
If the `NPC file was opened`, select an NPC from the `NPC List` pannel the view the details about the NPC.  
If the `gossip file was opened`, select a gossip instance from the `Gossip List` pannel the view the details about the gossip.  


## Editing XML files
Currently `gossipSim` can only read `NPC_Data.xml` and `Gossip_Data.xml`.  
So to effect the simulation `NPC_Data.xml` and `Gossip_Data.xml` need to be changed/updated.  

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
Any value `less than or equal to 0` is classed as a `negative` relation.  

---

Gossip events can be Edited in the `Gossip_Data.xml` file.  
Gossip type include: `positive`, `negative` and `neutral`.  
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

## Notes (Simulation)
Currently as the `spreading` of gossip can happen fast, if you want to see the `purple highlighting` update as gossip is passed around, you will need to click on the `gossip instance` as soon as it appears in the `Gossip List`.  

Sometimes a `graphical artifact` can appear, I think this is due to the renderer reading uninitialized memory (havent had time to fix yet).  
To fix the artifcating close an reopen the program (might need to do a few times).  


## Installation (Uncompiled Project)

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