/*
********************************************************************
Updates for gossip_meu_option
********************************************************************
*/

DELETE FROM `gossip_menu_option` WHERE `entry` BETWEEN 434 AND 507;
INSERT INTO `gossip_menu_option` (`entry`, `option_text`) VALUES
   (435, 'We are ready to defend the Alliance base.'),
   (436, 'The defenses are holding up: we can continue.'),
   (437, 'We\'re here to help! The Alliance are overrun.'),
   (438, 'We\'re okay so far. Let\'s do this!'),
   (439, 'Insult Coren Direbrew\'s brew.'),
   (440, 'Fight.'),
   (441, 'Apologize.'),
   (442, 'Let\'s go!'),
   (443, 'I will slay those Fanglords'),
   (444, 'We\'re ready to face Illidan.'),
   (445, 'I\'m ready, Akama.'),
   (446, 'I\'ll take the flight.'),
   (447, 'Borean Tundra'),
   (448, 'Howling Fjord'),
   (449, 'Sholazar Basin'),
   (450, 'Icecrown'),
   (451, 'Storm Peaks'),
   (452, 'Underground...'),
   (453, 'I need to speak with Corporal.'),
   (454, 'Warn him'),
   (455, 'You have fought well, spirit. I ask you to grand me the strenght of your body and the strenght of your heart.'),
   (456, 'What do you represent, spirit?'),
   (457, 'I seek to understand the importance of strength of the body.'),
   (458, 'I seek to understand the importance of strength of the heart.'),
   (459, 'I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw.'),
   (460, 'I need a moment of your time, Sir'),
   (461, 'Why... yes, of course. I\'ve something to show you right inside this building. Mr. Anvilward.'),
   (462, 'Release Him.'),
   (463, 'Walk free, Elder. Bring the spirits back to your tribe.'),
   (464, 'Where would you like to fly too?'),
   (465, 'Greer, i need a Gryphon to ride and some bombs to drop on New Agamand!'),
   (466, 'Speaking of action, I\'ve been ordered to undertake an air strike.'),
   (467, 'I need to intercept the Dawnblade reinforcements.'),
   (468, '<Ride the dragonhawk to Sun\'s Reach>'),
   (469, 'Why are Boulderfist out this far? You know this is Kurenai territory!'),
   (470, 'I\'m ready'),
   (471, 'I am listening, Dragon'),
   (472, 'But you are Dragons! How could orcs do this to you?'),
   (473, 'Your mate?'),
   (474, 'I have battled many beasts, Dragon. I will help you.'),
   (475, 'Examine the corpse'),
   (476, 'Care to try Grimbooze Thunderbrew\'s Jungle punch?'),
   (477, 'Hello, Rutgar. The Commander has sent me here to gather some information about his missing wife.'),
   (478, 'That sounds dangerous.'),
   (479, 'What happened to her after that?'),
   (480, 'Natalia?'),
   (481, 'What demands?'),
   (482, 'Lost it? What do you mean?'),
   (483, 'Possessed by what?'),
   (484, 'I\'ll be back once I straighten this mess out.'),
   (485, 'Hello, Frankal. I\'ve heard that you might have some information as to the whereabouts of Mistress Natalia Mar\'alith.'),
   (486, 'That\'s what I like to hear.'),
   (487, 'That\'s odd.'),
   (488, 'You couldn\'t handle a lone night elf priestess?'),
   (489, 'I\'ve been meaning to ask you about that monkey.'),
   (490, 'Then what?'),
   (491, 'What a story! So she went into Hive\'Regal and that was the last you saw of her?'),
   (492, 'Thanks for the information, Frankal.'),
   (493, 'Take Insignia'),
   (494, 'I\'m ready, Summon Him!'),
   (495, 'Goodbye'),
   (496, 'Please take me to the Master\'s Lair'),
   (497, 'Try this'),
   (498, 'I inspect the body further.'),
   (499, 'Are you okay? I\'ve come to take you back to Frosthold if you can stand.'),
   (500, 'I\'m sorry that I didn\'t get here sooner. What happened?'),
   (501, 'I\'ll go get some help. Hang in there.'),
   (502, 'Offer treat'),
   (503, 'I\'m a messenger for Draenei'),
   (504, 'Get message'),
   (505, 'Lady Jaina, this may sound like an odd request... but I have a young ward who is quite shy. You are a hero to him, and he asked me to get your autograph.'),
   (506, 'Give me hoofprint.');
   
UPDATE `world_db_version` SET `LastUpdate` = '2015-04-06_01_gossip_menu_option';
