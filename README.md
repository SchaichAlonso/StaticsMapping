# Introduction
Before version 10.50, the only way for an XPlane airport to contain static scenery aircraft was by the airport authors manually placing these static objects during the airport implementation. For this purpose, several XPlane scenery library packages containing static aircraft objects that can be used exist at the point of this writing. However, the many libraries use different standards (or no standards at all) in terms of how to name, position, align, ... aircraft objects. As these objects are manually placed by airport authors, it becomes the airport author's responsibility to adapt object attributes to fit the author's requirements.

This system had significant limitations. Referencing the static aircraft library in the airport scenery effectively makes the static library a dependency of that airport. This dependency, or rather these dependencies (as multiple aircraft objects are picked from different object libraries), needs to be taken care of by the end user. However, as time passes new versions of the object libraries are released, and compatibility is often not preserved. Further, while XPlane lacks collision detection technology, estitiques demand for the user not to &quot;use&quot; a gate already occupied by a static aircraft object, which means that either are the airports only populated very densely, or a massive number of parking positions is lost. Even if only dense population is around, the static airliners can be occupying a parking position interesting to the end user.

XPlane 10.50 overhauled this system, introducing airport *start* usage identifiers. Using WED-1.5 or newer, each *start* can be configured a list of airline identifiers in addition to the startup equipment available at that *start*. When XPlane initializes an airport scenery, for each *start* not occupied by the user, it has some chance to pick a random aircraft type supported by the *start* 's equipment. These randomly picked aircrafts are assigned airline identifiers according to the *start* specifications. If a usable aircraft object is provided by any scenery library, XPlane will use it to populate the corresponding *start*. Otherwise XPlane drops the airline identifier and searches the library for a compatible object for the aircraft type independend of the object's livery.

In the real world, airport gates and parking positions are leased to operators. These operators are rarely independend but usually somehow related with other operators (subsidies, aliances, ...) and offer their gates to their relatives at a discounted rate or for free. Also, if a large gate is available to an operator, rather than leasing a new smaller gate, operators often opt to use the large gate for a &quot;small&quot; plane: Gates designed to service Jumbos can regularily be seen to service Regional Jets instead.

An operator giving a gate to a &quot;random&quot; competitor, however, is quite rare.

<br/>
<br/>

The goal of this project is to generate a standartized XPlane-10.50/XPlane-11 aircraft objects library. It does so by locating user-installed aircraft objects, transforming them to conform to XPlane-10.50's requirements, and exposing each object to all XPlane identifiers that it is the &quot;best fit&quot; for.


# Usage

### Generating airport metadata

This project tracks airline regional presence by storing airports that serve as hubs to airlines. The regional distance weight (not interfaced at this time) to traverse from one airline to another is the distance, in commenced 100km invervals, of the closest two airports assigned to them.

For licencing reasons, the airport data from XPlane cannot be distributed with this package. An application called **AptDatExtract** can be used to extract the information from an xplane installation instead.

![Windows](https://github.com/xibo/StaticsMapping/raw/master/doc/AptDatExtract.jpeg)

In the screenshot, it is instructed to use the airport data of the XPlane installation in **c:\XPlane10**.

Hitting the *go* button will cause **AptDatExtract** to generate it's airport metadata. If it is being run from the command line, progress diagnostics will be emited to standard output (the GUI freezes during the progress). On completion or failure, **AptDatExtract** will open a Dialog to report termination.

![Windows](https://github.com/xibo/StaticsMapping/raw/master/doc/AptDatExtract-Success.jpeg)

**AptDatExtract** can extract metadata only for Airports in XPlane-10.50 format. Older airports will fail to have their metadata extracted, despite being present in the XPlane installation, and only contents of the global default scenery, as in gateway scenery, will be processed. No means are provided within the scope of this project to modify the airport metadata. Either update the airport on the scenery gateway, or edit *Data/airports.json* by hand.

This airport data needs to be extracted only once, though it might be advantageous to re-extract it after new default airports were introduced by an XPlane update.



### Generating the object files

The **LibraryWizard** application is used to search for known statics on the filesystem.

![LibraryWizard Intro](https://github.com/xibo/StaticsMapping/raw/master/doc/LibraryWizard-Intro.jpeg)

It analyzes all files within the user provided *Scan Path*, recursing into any subdirectories. The input path is not required to be part of an XPlane installation.

The *Output Path* specifies where to generate objects in. Once again, it is not required to be within an XPlane installation.

Each known object file discovered while recursing the *Scan Path* will be loaded, fixed to conform the XPlane-10.50 static aircraft object requirements, and stored in the *Output Path*. The discovered objects files themselves will not be modified, all required transformations are done in memory and then written to the *Output Path*, which needs to have been created manually (use the *browse* button to open a dialog that provides the option to create a new directory). The objects stored in the *Output path* will not have any dependencies to anything outside the *Output Path*.

The *Worker Threads* tunable controls how many objects will be worked on concurrently. It serves mostly debugging purposes and the default value should work well enough on most systems.

The *Texture Resolution Limit* can be used to restrict the size of textures used by aircraft statics. Limiting it will compromize visual quality of the produced statics, but will reduce resource consumption by XPlane on runtime.

Press *Next* to have the object files be scanned. The following page will emit the scan progress.

![LibraryWizard Intro](https://github.com/xibo/StaticsMapping/raw/master/doc/LibraryWizard-Scaning.jpeg)

On completion, a file named *found.json* will be generated in the output path. It stores metadata and can be used as input for further runs.

<hr/>

If the **LibraryWizard** application had already been used to populate an *Output Path*, that output can be reused, i.e. to regenerate the XPlane library without having to do the time-exhaustive object file scan/transform/write. Select the *Reuse Old* Radio Button and find the *found.json* file stored in the *Output Path*.


### Generating the XPlane library

Having generated the library, or having been provided an previous run's *found.json* file, the **LibraryWizard** application can be continued to the XPlane Library Options page.

![LibraryWizard Intro](https://github.com/xibo/StaticsMapping/raw/master/doc/LibraryWizard-Afiliates.jpeg)

In the *Preview* group that covers bottom half of the page, an airline can be selected with the ComboBox next to the *Operator* label, to have it's available objects be listed.
In the object listing itself, *Object* is the filename within the *Output Path*, *Weight* is a metric that describes how difficult it is for the selected airline to access that object, and *VPath* is the XPlane LIBRARY format &lt;vpath&gt; prefix that will be used.

The *Preview Selected* button can be used to look inside any of the airline's objects, after having selected it.

<hr/>
Press the &quot;write library&quot; button below the *Preview* group to have the XPlane library be written with the current settings.

<hr/>
In the *Options* group that covers the upper half of the page, a number of settings can be configured to tune how the *Weights* are computed. If multiple objects are available, always the object with the lowest *weight* will be used. If multiple objects share the lowest *weight*, each of them will be used.

When walking in the operator-subsidy hierarchy, the *weight* is the sum of the traversed relations, each weighted with either *Parent-&gt;Child* or *Parent&lt;-Child* according to the relationship.

Using a smaller aircraft for a large *start* is weighted by
<pre>d*d*q + d*l</pre>
where *d* is the size difference, *q* is *Size-up quadratic weight* and *l* is *Size-up linear weight*.

*Current Year* imposes a filter that disqualifies objects that are associated with a different age. A setting of 0 enables objects independend of their lifetime.
![AFL-1987](https://github.com/xibo/StaticsMapping/raw/master/doc/AFL-1987.jpeg)
![AFL-2017](https://github.com/xibo/StaticsMapping/raw/master/doc/AFL-2017.jpeg)
In the first image above, the *Current Year* setting is used to enable &quot;history mode&quot; and limit Aeroflot Russian Airlines (AFL) to the fleet and paintjobs it used in 1987, while AFL was restricted to the limits of it's current fleet and paints in the second image. If the year was 0, AFL would be allowed to use both it's old and it's new objects.

*Max. Plane Age* only has an effect if *Current Year* is also set. It removes objects not introduced later than *Current Year* - *Max Plane Age*. Note that some objects have introduction dates that are far more recent than the object: A 1935-built DC-3 painted in American Airline's 2013 livery is handled like an aircraft built in 2013.

*First Uses* specifies what LIBRARY instruction will be used for the first object of each name. The default of EXPORT_EXTEND should &quot;just work&quot; on most installations. If EXPORT is selected instead, previous objects of the same name will not be used by XPlane, however the word &quot;previous&quot; comes at the &quot;usual&quot; implications.



### Installing the XPlane library

After having generated an XPlane library, the *Output Path* has to be copied to XPlane's
<pre>Custom Scenery</pre>
subdirectory, unless it is already there.

If *First Uses* was set to EXPORT, *scenery_packs.ini* will likely have to be adapted.

# Modifying the Dataset

The object metadata definitions are stored in *Data/data.json* (and *Data/airports.json*). The later file is not shipped by this project, but has to be extracted from an XPlane installation as already explained.

The primary program to modify the dataset definitions is *TagTool*.

![TagTool](https://github.com/xibo/StaticsMapping/raw/master/doc/TagTool.jpeg)

Upon startup, it loads both the object and the airport metadata. It will never write back it's in-memory state of them unless explicitly asked to do so using Definitions-&gt;Save from the Menu.</p>

Object files can be loaded either by passing them as command line parameters, using Object-&gt;Open from the Menu, or drag-and-dropping them from a filesystem browser. All object files loaded will immediately have new metadata for it be added to the in-memory dataset.

If this is undesired, a second application, called ObjPreview, can be used in the same way, however it cannot view, edit, or store metadata. To load unknown objects in order to find out what they contain, use ObjPreview or WED instead.

![ObjPreview Lod=0m](https://github.com/xibo/StaticsMapping/raw/master/doc/ObjPreview1.jpeg)
As XPlane does not deploy any geometry simplification technology, all LOD data needs to be precomputed offline, or &quot;baked into the obj-file&quot; in LR-speak. If the object file defines LOD ranges, the slider on the right of the preview of either ObjPreview or TagTool can be used to configure the &quot;camera distance&quot; of the preview.

![ObjPreview Lod=1200m](https://github.com/xibo/StaticsMapping/raw/master/doc/ObjPreview2.jpeg)
Moving up the slider shows that RuScenery's mig29 will look rather ugly from more than a kilometer away (1.2km in the screenshot), and unrecognize from more than 4km away.

In TagTool, a newly loaded object will not be displayed automatically. It needs to manually be selected using the &quot;displayed&quot; combobox. Selecting an object will cause it to be rendered in the secondary window, and all metadata editables will switch to display the metadata associated with this object file. **Please make sure the metadata is correct***.

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
