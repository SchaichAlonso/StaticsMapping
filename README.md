# Introduction
Before version 10.50, x-plane static scenery aircraft used to be manually placed by airport authors as part of the airport implementation. Several x-plane scenery library packages containing static aircraft objects that can be used for this purpose exist at the point of this writing. The many libraries use different standards (or no standards at all) as of what an aircraft is named, positioned, aligned, and so on. As the objects are manually placed by airport authors, it becomes the airport author's responsibility to check whatever standard is used by the object being used fits the author's requirements.

This system has significant limitations. First, in order to be able to use the airport, all packages corresponding to aircraft objects manually placed have to be installed on the system in a compatible version, which quickly causes some dependency (and deep-endency) creep to be taken care of by end users. Further, as airports are manually populated at design-time, they will always be populated by the same objects at run-time.

X-Plane 10.50 overhauled this system, introducing gate/start usage identifiers. Using WED-1.5 or newer, each start can be configured a list of airline identifiers. At run-time, X-Plane will pick a random airline identifier and eventually populate the start with a plane registered to that airline that can otherwise be operated at that start (i.e. has compatible size and engines).

However, should X-Plane randomly attempt to select an object which it does not have in any library, rather than repicking a different airline, X-Plane populates the start with a random aircraft instead.

In the real world, gates are usually owned by airport operators, which lease it to an airline, that either uses the gate itself, or allows it's codeshare partners, subsidies, or alliance partners to use the gate in it's stead. Further, while X-Plane tries to populate starts with the largest possible plane, in the real world small aircrafts are parked on large parking positions. After all, the airline already owns the (large) gate, so not using it would be a waste.

The goal of this project is to generate a standartized XPlane-10.50/XPlane-11 aircraft statics library by transforming static object package contents provided by other means, and exposing each object to all airlines it is the "best fit" for.

<br/>
<br/>
# Usage

### Generating airport metadata

This tool tracks airline regional presence by storing airports that serve as hubs to airlines. In regional fallback mode (not interfaced at this time), the &quot;distance&quot; between two airlines is defined between the closest two airports assigned to them.

For licencing reasons, the airport data from xplane cannot be distributed with this package. An application called <i>AptDatExtract</i> can be used to extract the information from an xplane installation instead. It receives the path to the XPlane global airport definitions file as a command line argument, and generates the airport metadata file to be later processed. From the XPlane installation prefix, the default airports definitions are installed in <pre>Resources/default scenery/default apt dat/Earth nav data/apt.dat</pre>.

![Unix](https://github.com/xibo/StaticsMapping/raw/master/doc//AptDatExtract-Unix.jpeg)
In the example setup, XPlane is installed in <i>/compat/linux/XP10</i>, and the path above is appended. Care needs to be taken about the space characters.
TODO windows image

<i>AptDatExtract</i> can extract metadata only for Airports in XPlane-10.50 format, so older airports might fail to have their metadata extracted despite being present in the XPlane installation. No means are provided within the scope of this project to edit the airport metadata. Either update the airport on the scenery gateway, or edit <i>Data/airports.json</i> by hand.

This airport data needs to be extracted only once, though it might be advantageous to re-extract it after new default airports were introduced by an XPlane update.



### Generating the object files

The Scanner application is used to search for known statics on the disk.

![Screenshot](https://github.com/xibo/StaticsMapping/raw/master/doc//Scanner.jpeg)

It analyzes all files within the user provided input path, recursing into any subdirectories. The input path is not required to be part of an XPlane installation. In the screenshot, the default download path is used.

The output path specifies where to generate objects in. Once again, it is not required to be within an XPlane installation path.

Each known object found while recursing the input path will be loaded, fixed to conform the XPlane-10.50 static aircraft object requirements, and stored in the output path. The input itself will not be modified, all required transformations are done in memory and then written to the output path. The objects stored in the output path do not have any dependencies to anything outside the output path.

The Workers tunable controls how many threads will be used by the scanner. It serves mostly debugging purposes and the default value should work well enough on most systems.

The Texture resolution limit can be used to restrict the size of textures used by aircraft statics. Limiting it will compromize visual quality of the produced statics, but will preserve system and graphics memory as well as reduce load times of XPlane.

Press scan to have the object files be scanned. On unix, a diagnostic is emited on the command line for each found object. The resulting objects are created directly within the output directory and are named
<pre>a-b-c.obj</pre>
where a is the corresponding aircraft's ICAO code, b is the ICAO code of the airline associated with the paintjob, and c is a counter that is used to distinguish multiple statics of the same aircraft and airline.

The Scanner application also generates a file called <i>found.json</i>, which contains metadata for the files in the output path.



### Generating the XPlane library

The <i>Library</i> application is used to generate an XPlane-10.50 static aircraft object export definition file. It is run from the command line and is passed the <i>found.json</i> file that was emited by <i>Scanner</i> earlier. It will generate a file named <i>library.txt</i> next to the <i>found.json</i> file.

To register the newly created library to XPlane, copy the output path into
<pre>Custom Scenery</pre>
below the XPlane installation path, and restart XPlane. Manual modifications of the <i>scenery_packs.ini</i> file should not be required.



The Library application currently lacks all tunables and will generate entries with the default setup only, which is exclusive airline hierarchy distribution and exporting fictive and outdated objects.



<div style="border-style: dotted;">
<h3>Example 1</h3>
The Scanner finds one aircraft object, an Sukhoi Superjet painted in Aeroflot paint scheme.<br/>
The Superjet is exported to become available to any member of the SkyTeam alliance that Aeroflot is part of, as well as any subsidy of each SkyTeam member, the subsidies of each SkyTeam member's subsidy, and so on.<br/>


<h3>Example 2</h3>
Like Example 1, but an Boeing 737 painted in Delta Airlines paint scheme is also detected by the scanner.<br/>
Delta exclusively uses it's 737 and Aeroflot exclusively uses it's Superjet. Because they are "more related" to Aeroflot than to Delta, Aeroflot's subsidy Rossiya, as well as Rossiya's subsidy OrenAir exclusively use the Superjet. All other (subsidies of) SkyTeam members are given access to both the 737 and the Superjet.<br/>


<h3>Example 3</h3>
Like Example 2, but an Boeing 777, painted in Aeroflot paint scheme is also available.<br/>
The 777 is a different size category than either the 737 or the Superjet. The smaller planes are exported as in example 2, however they are not exported to become available to "large" gates any more, which is where the 777 is now used.
</div>




<br/>
<br/>
# Modifying the Dataset</h3>

The object metadata definitions are stored in <i>Data/data.json</i> (and <i>Data/airports.json</i>). The later file is not shipped by this project, but has to be extracted from an X-Plane installation as already explained.

The primary program to modify the dataset definitions is <i>TagTool</i>.

![TagTool](https://github.com/xibo/StaticsMapping/raw/master/doc/TagTool.jpeg)

Upon startup, it loads both the object and the airport metadata. It will never write back it's in-memory state of them unless explicitly asked to do so using Definitions-&gt;Save from the Menu.</p>

Object files can be loaded either by passing them as command line parameters, using Object-&gt;Open from the Menu, or drag-and-dropping them from a filesystem browser. All object files loaded will immediately have new metadata for it be added to the in-memory dataset.

If this is undesired, a second application, called <i>ObjPreview</i>, can be used in the same way, however it cannot view, edit, or store metadata. To load unknown objects in order to find out what they contain, use <i>ObjPreview</i> (or WED) instead.

![ObjPreview Lod=0m](https://github.com/xibo/StaticsMapping/raw/master/doc/ObjPreview1.jpeg)
As XPlane does not deploy a geometry simplification technology, all LOD data needs to be precomputed offline (or &quot;baked&quot; in LR-speak). The slider on the right of the preview of either <i>ObjPreview</i> or <i>TagTool</i> can be used to configure the &quot;camera distance&quot; of the preview.

![ObjPreview Lod=1200m](https://github.com/xibo/StaticsMapping/raw/master/doc/ObjPreview2.jpeg)
Moving up the slider shows that RuScenery's mig29 will look rather ugly from more than a kilometer away, and unrecognizably poor from more than 4km away.

In <i>TagTool</i>, a newly loaded object will not be displayed automatically. It needs to manually be selected using the &quot;displayed&quot; combobox. Selecting an object will cause it to be rendered in the secondary window, and all metadata editables will switch to display the metadata associated with this object file. <b>Please make sure the metadata is correct</b>.

<br/>
<br/>
<h3>Object Metadata</h3>
<ul>
<li><p>The filename is a hint for humans and ignored, as is the comments.</p></li>

<li><p>The aircraft and livery comboboxes are used to indicate what aircraft and paintjob the object represents. If an object represents an GA static, select <i>XXX-GeneralAviation</i> from the livery combobox.  Available aircrafts and airlines can be reviewed by selecting <i>Definitions-&gt;Aircraft Table</i>, <i>Definitions-&gt;Airline Table</i> and <i>Definitions-&gt;Airline Hierarchies</i> from the menu. Airline Hierarchies is only available if the TagTool was built with GraphViz support.</p></li>

<li><p>The purpose combobox indicates what an object is used for. All commercial airline traffic should go "airliner", while commercial cargo planes should be "cargo". If a plane is used for both cargo and passenger transport, select "MultiPurpose", which will export the plane to both airline and cargo categories in X-Plane.</p></li>

<li><p>The library field indicates where the current object originates from. Available libraries can be reviewed by selecting Definitions-&gt;Libraries from the menu.</p></li>



<li><p>The introduction and retirement is an integer number representing the year the objects was in use. If it is not known, enter 0. The fictive toggle indicates whether the object represents a plane that ever existed. If it is unknown, assume a plane to be fictive.</p></li>

<li><p>A &quot;good&quot; object aircraft is one that has it's front wheel positioned on the coordinate locale, and has it's body's end toward the blue coordinate axis. Use the locale spinboxes and the rotation combobox in order to ensure this is provided in the preview (if the preview isn't updating automatically, hit enter in the spinboxes or select the comments field).</p></li>


<li><p>Once an object's metadata was completely edited, the in-memory dataset can be stored to disk using <i>Definitions-&gt;Save</i> from the menu.</p></li>
</ul>

<p>Use <i>git diff</i> to review the changes just done to the file, and commit and push them using git and post a merge request in order to share them with upstream development (See github guides on how to do that).</p>


<br/>
<br/>
<h3>Airline Metadata</h3>
<ul>
<li>
<p>Aircraft operators are identified by their ICAO code. It is usually an &quot;Airline ICAO&quot; code, though some military or government operators also have codes. All real-world ICAO airline codes are 3 ascii alphabets long, though the system accepts fictive ICAO codes, too. Fictive ICAO codes have a <i>XXX-</i> prefix and an arbitrary length and composition.</p>
<div style="border-style: dotted;">
<h5>Example</h5>
<p><i>American Airlines</i> has ICAO code <i>AAL</i>.</p>
<p>It is member of the OneWorld Alliance, which is not an operator itself and therefore has no ICAO code. However, to represent the OneWorld entity, a fictive <i>XXX-OneWorld</i> ICAO code is used.</p>
</div>
</li>
<li>
<p>Airline names, and comments, are for human interfacing only, and ignored.</p>
<div style="border-style: dotted;">
<h5>Example</h5>
<p><i>American Airlines</i> has ICAO code <i>AAL</i>.</p>
<p>The name field corresponding to <i>AAL</i> is input <i>American Airlines</i>, and a comment is omited.</p>
</div>
</li>
<li>
<p>hubs and missing hubs represent hub airports used by the airline. They are given as their airport ICAO codes, and codes not found in the airport dataset (from <i>airports.json</i>) are automatically moved to missing hubs in the GUI. In the on-disk data format, there is no distinction between present and missing airports, so airports found in future versions of xplane will not require modification of the airline data.</p>
<div style="border-style: dotted;">
<h5>Example</h5>
<p><i>American Airlines</i> operates hubs in Charlotte (Icao: KCLT), Washington (KDCA), Dallas (KDFW), New York JFK (KJFK), New York La Guardia (KLGA), Los Angeles (KLAX), Miami (KMIA), Chicago (KORD), Philladelphia (KPHL), Phoenix (KPDX), London (EGLL) and Tokyo (RJAA).</p>
<p>However, <i>AAL</i> is a representative airline only for the airports in the USA and neighboring countries, so London and Tokyo are not registered as hubs. Instead <i>KCLT KDCA KDFW KJFK KLAX KLGA KMIA KORD KPHL KPDX</i> is input to the hubs field.</p>
<p>XPlane-11.01 provides an airport for Phoenix, however it lacks XPlane-10.50 format's metadata and was therefore not scanned by <i>AptDatExtract</i>. When finishing input into <i>AAL</i>'s hubs field, the <i>KPDX</i> indicator is automatically moved to the missing hubs field.</p>
</div>
</li>
<li>
<p><i>founded</i> and <i>ceased</i> are integer numbers indicating the year an airline was founded or ceased. If unknown, should be 0. These are used as fallback for objects that do not provide their own <i>introduction</i> and <i>retirement</i> numbers.</p>
<div style="border-style: dotted;">
<h5>Example</h5>
<p>Object <i>x</i> does not specify it's retirement (i.e. <i>retired=0</i>) but is painted in a special livery introduced in 2014 and operated by Air Berlin (Icao: <i>BER</i>). <i>BER</i> was founded in 1979 and ceased operations in 2017. <i>x</i> introduction is provided, but it's retirement is implied from <i>BER</i>'s cease, and therefore, if age filtering is enabled, <i>x</i> is available from 2014 to 2017.</p>
</div>
</li>
<li>
<p><i>owner</i> is the ICAO code of an airline that owns this airline. If this airline is not owned by anyone else, it's owner is it's own ICAO code.
<div style="border-style: dotted;">
<h5>Example</h5>
<p>Air Wisconsin (AWI) operates flights for American Airlines (AAL), which is a member of the OneWorld alliance (XXX-OneWorld, see above).</p>
<p><i>AWI</i>'s owner field is set to <i>AAL</i>, <i>AAL</i>'s owner field is set to <i>XXX-OneWorld</i>, and <i>XXX-OneWorld</i>'s owner is set to <i>XXX-Oneworld</i>.</p>

<h5>Example</h5>
<p>Royal Brunei Airlines (BRA) is independend.</p>
<p><i>BRA</i>'s owner field is set to <i>BRA</i></p>
</div>
</li>
</ul>


<br/>
<br/>
<h3>Aircraft Metadata</h3>
<ul>
<li>
<p>Aircraft are identified by their <i>ICAO</i> records, like airlines. However, no fictive ICAO codes are allowed here. See docs/faa-aircraft.pdf for a list of ICAO aircraft codes.</p>
</li>
<li>
<p><i>engine</i> is the plane's propulsion system. It must be one of:</p>
<ul>
<li>Jet</li>
<li>Turbofan</li>
<li>Turboprop</li>
<li>Piston</li>
<li>Helicopter</li>
</ul>
Gliders, electric planes, Baloons, and so on, are not supported at this time.
</li>
<li>
<p><i>length</i> is the length of the plane, in meters. In the on-disk format (json), it is stored in millimeters instead.</p>
</li>
<li>
<p><i>wingspan</i> is the width of the plane, in meters. In the on-disk format (json), it is stored in millimeters instead.</p>
</li>
<li>
<p><i>mtow</i> is the maximal allowed takeoff weight of the plane, in metric tons. In the on-disk format (json), it is stored in kilogrammes instead.</p>
</li>
<li>
<p><i>omgws</i> is the plane's outer main gear wheel span, in meters, or 0 if unknown. In the on-disk format (json), it is stored in millimeters instead.</p>
</li>
<li>
<p><i>introduction</i> is an integer representing the year of the first flight of the plane. It is used for objects that do not imply their own introduction year.</p>
</li>
</ul>
<div style="border-style: dotted;">
<h5>Example</h5>
<p>A Boeing 757-200 is a single aisle, twin engine turbofan plane with a MTOW of 115.65t, a wingspan of 38.05m, a length of 47.32m and a wheelspan of 8.55m that made it's maiden flight in 1983. It's ICAO code is B752.</p>
<p>Correctly assigned, all objects that are assigned <i>B752</i> into their aircraft field will be restricted for usage after 1983 only (if age restriction is enabled), and will be assigned to xplane's <i>heavy_d</i> class.</p>
</div>


<br/>
<br/>
<h3>Compiling</h3>
<p>To compile the application from sources, a C++-11 compiler, Qt-5.6 and cmake are needed.</p>
<p>If found, libgvc is also used. On unix, in an empty directory type
<pre>
cmake ${source_dir}
make
</pre>
to build everything. Replace ${source_dir} by the root of the source tree.</p>
<p>On win32, using visual c++, use
<pre>
cmake -G"NMake Makefiles" ${source_dir}
nmake
</pre>
instead, after ensuring qmake.exe is in the PATH.</p>
