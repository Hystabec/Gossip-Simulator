# Gossip-Simulator

Only `Windows` is supported.  
Compiled program can be found on the [Release Tab](https://github.com/Hystabec/Gossip-Simulator/releases).  
Built using my game engine [DaedalusEngine](https://github.com/Hystabec/DaedalusEngine).  

The project contains 2 application `gossipSim` and `gossipSim-nodeEditor`.  
`gossipSim` is the main application that runs the simulator.  
`gossipSim-nodeEditor` is a helper program that allows the user to `view and edit` the XML files (`NPC_Data.xml`, `Gossip_Data.xml`) with a GUI.  

## Simulation Description
The number of NPCs and their relationships is defined in `NPC_Data.xml`.  

Each `NPC Tick` (also called `Update Tick` or `Tick`) (`2 seconds`) the NPCs will be updated, unless the simulation is `paused`.  
Each `NPC Tick` if a gossip instance is to start on that tick (specified in `Gossip_Data.xml`), the gossip will be given to the NPC and then they will start spreading it `next tick`.  
When an NPC hears gossip the will wait `1 tick` before spreading it.  

Gossip has 3 types:  
1. `Positive`
2. `Negative`
3. `Neutral`

NPCs can either have a `Positive` or `Negative` relationship with other NPCs.  

IF the NPC has heard a gossip, but is yet to tell it, they will attempt tell the NPCs they know:  
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

The NPCs are loaded from `NPC_Data.xml` when the program is started an will be renderer to the screen.  
The gossip instances are loaded from `Gossip_Data.xml` and will occur on the `startTick` specified in the file, the start tick relates to the `NPCs ticked counter` on the `NPC Details` pannel.  
NPCs will wait `1 NPC tick` after hearing a gossip instance before the start spreading it.  

The `console` will show information about: 
1. when a new gossip instance is started
2. when an NPC start telling gossip
3. When an NPC tells another NPC gossip and the outcome of the gossip telling (remembered or ignored), with the reason for the outcome.  

The `window` shows all the NPCs (white circles as default) as well as 4 panels `Simulator Controls`, `NPC Details`, `Gossip Selector` and `Gossip Details`.  

The `Camera` can be moved using `W A S D`.  
When a `NPC is hovered`, with the mouse cursor, the `hovered NPC` will turn `gray` and the `other NPCs` will `change colour` to reflect their relationships with the other NPC `Green is liked`, `Red is disliked` and `Stay white if they don't know them`.  

### Simulator Controls Panel
The `Simulator Controls` panel allow the user to control the simulator.  
This panel alows the user to:  
* Play and Pause the simulation  
* Manually move to the next update tick  
* View the current update tick  
* View the time until the next update tick  
* Restart the simulation  

The `Play` button (which become `Pause` while the simulator is running) allows the user to start and stop the simulation.  
The `Next Tick` button allows the user to manually move the simulation to the next update tick (can be used while the simulation is both running or paused).  
The `Restart` button allows the user the restart the simulation to the beginning.  

### NPC Details Panel
The `NPC Details` panel shows the `NPCs ticked counter` this counter is updated each time the NPCs are ticked, this counter is also show on the `Simulator Controls` panel as the `Current Tick` counter.  
While an NPC is hovered (with the mouse cursor), the `NPC Details` panel will display the details of that NPC.  
Details shown include:  
1. The name of the NPC  
2. The personality of the NPC  
3. A relationship table  

The `relationship table` show all the other NPCs that the hovered NPC knows as well as if the relationship is `Positive` or `Negative`.  

### Gossip Selector Panel
The `Gossip Selector` panel contains a list of the currently active gossip instances, selecting a gossip from the list will cause the NPCs that have heard the gossip to `highlight in purple`.  
The `Gossip Details` panel will also show details of the selected gossip.  

### Gossip Details Panel
The `Gossip Details` panel (by default) will show a message telling the user to select a gossip instance from the `Gossip Selector` panel.  
Once a gossip instance has been selected the details of the gossip will be displayed.  
These details are:  
* The ID of the gossip (which is specified in `Gossip_Data.xml`)  
* The name of the NPC that the gossip is about  
* The type of the gossip (`Positive`, `Negative` or `Neutral`)  

The `Event List` show the tick the gossip started on and the NPC that started it.  
The `Event List` also has a table that contains each of the events of the gossip.  
A `gossip event` consists of:
* The tick the gossip occured  
* The name of the NPC that was spreading the gossip  
* The name of the NPC that heard the gossip  
* The outcome of the gossip (If it was `Remembered` or `Ignored`)  

If the `(?)` is hovered over on the `Outcome` result the reason for remebering or ignore the gossip will be displayed.  

-----

`gossipSim-nodeEditor` is a helper program that allow the `viewing and editing` of the contents of the XML data files with a UI.  
When executed a `Window` will spawn.  

The `Window` will initially show no data a will display the text:  
```
No file selected
Select the XML file to modify (File -> Open)
```
To open an XML file navigate to the `File` menu on the `Menu Bar` and select `Open`.  
Once selected a `File Open Dialog` will appear, then open either `NPC_Data.xml` or `Gossip_Data.xml`.  

This program has `NO AUTOMATIC SAVING`, so if you want to save any changes that you have made, navigate to the `File` menu on the `Menu Bar` and select `Save`, which will save the the file that was being viewed.  

### NPC File Navigation
If an `NPC` file was opened.  
The `NPC List` panel will display all the NPC within the file.  
The `NPC List` panel will remain empty if no NPC were found in the file.  
A `new NPC can be added` by navigating to the `Edit` menu on the `Menu Bar` and selecting `New NPC`.  
Once an `NPC` from the `NPC List` panel is selected data about the selected NPC will be show on the `NPC Data` panel.  
In the `NPC Data` panel values about the NPC can be `View and Edited`.  
The `NPC name` cannot currently be Edited inside of the program and will need to be done manually in the XML file.  
The `Personality` can be edited by selecting either `none`, `spreader` or `sink` from the `drop down menu`, the `drop down menu` can be opened by clicking on the current personality.  
The `Relationship Table` show all the relationships of the currently selected NPC.  
The `Add` a new relationship navigate to the `Edit` menu on the `Menu Bar` and select `Add NPC Relationship`, this will add a blank element into the table.  
The `NPC name` on the `relationships table` can be edited by clicking in the name box and typing the new name.  
The `Relationship` on the `relationships table` can be edited by clikcing on the relation box and selecting either `positive` or `negative` from the `drop down menu`.  
The currently selected NPC can be `deleted` by navigating to the `Edit` menu on the `Menu Bar` and selecting `Delete Selected Item`.  

### Gossip File Navigation
If a `Gossip` file was opened.  
The `Gossip List` panel will display all the gossip instances within the file.  
The `Gossip List` panel will remain empty if no gossip instances were found in the file.  
A `new gossip can be added` by navigating to the `Edit` menu on the `Menu Bar` and select `New Gossip`.  
Once a `Gossip` from the `Gossip List` panel is selected data about the selected gossip instance will be show on the `Gossip Data` panel.  
In the `Gossip Data` panel values about the gossip can be `View and Edited`.  
The `gossip name` cannot currently be Edited inside of the program and will need to be done manually in the XML file.  
The gossip `Type` can be edited by selecting either `positive`, `negative` or `neutral` from the `drop down menu`, the `drop down menu` can be opened by clicking on the current type.  
The `NPC` the gossip is `about` can be edited by clicking in the box and typing the new NPC name.  
The `NPC` the gossip is `starting from` can be edited by clicking in the box and typing the new NPC name.  
The `start tick` for the gossip can be edited by clicking in the box and typing the new start tick (as an interger).  
The currently selected gossip can be `deleted` by navigating to the `Edit` menu on the `Menu Bar` and selecting `Delete Selected Item`.  

If you want to `save` any changes that have been made to the current file, navigate to the `File` menu on the `Menu Bar` and select `Save`.  

## Editing XML files
Currently `gossipSim` can only read `NPC_Data.xml` and `Gossip_Data.xml`.  
So to effect the simulation `NPC_Data.xml` and `Gossip_Data.xml` need to be changed/updated.  
The files can either be changed using `gossipSim-nodeEditor` or can be changed manually.  

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
NPCs can have a specified personality `spreader` or `sink`, if no personality is specified the will follow the default rules.  
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

Sometimes a `graphical artifact` can appear, I think this is due to the renderer reading uninitialized memory (haven't had time to fix yet).  
To fix the artifacting close an reopen the program (might need to do a few times).  

I wanted to add arrows between each of the nodes to show relationship as well as the spread of gossip, however the engine i'm using (My custom game engine) had issue rending the arrows at certain angles and it cause the whole thing to look more confusing so i decided against it.  


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