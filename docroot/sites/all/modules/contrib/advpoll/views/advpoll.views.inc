<?php

/*
 * Implementation of hook_views_data_alter
 */

function advpoll_views_data_alter(&$views_data) {
// Borrowed structure from votingapi poll since this poll relies upon its 
// behavior to tie votes to node.
  $default_relationships[] = array(
    'description' => t('Advanced Poll'),
    'entity_type' => 'advpoll',
    'base_table' => 'node',
    'entity_id_column' => 'nid',
    'pseudo_vote' => 'votingapi_vote', // for legacy compatability w/RC1.
    'pseudo_cache' => 'votingapi_cache', // for legacy compatability w/RC1.
  );  
  
foreach ($default_relationships as $data) {
    $pseudo = str_replace(array(' ','-','.'), '_', $data['entity_type'] . '_' . $data['entity_id_column']);
    $pseudo_vote = empty($data['pseudo_vote']) ? 'vapi_'. $pseudo : $data['pseudo_vote'];
    $pseudo_cache = empty($data['pseudo_cache']) ? 'vapic_'. $pseudo : $data['pseudo_cache'];
    
    $views_data[$data['base_table']][$pseudo_vote]['relationship'] = array(
      'title' => 'Votes',
      'help' => 'Votes cast by users on ' . $data['description'] . '.',
      'base' => 'votingapi_vote',
      'field' => 'entity_id',
      'relationship field' => $data['entity_id_column'],
      'handler' => 'votingapi_views_handler_relationship',
      'extra' => array(
        array(
          'field' => 'entity_type',
          'value' => $data['entity_type'],
          'numeric' => FALSE
        ),
      ),
    );

    $views_data[$data['base_table']][$pseudo_cache]['relationship'] = array(
      'title' => 'Vote results',
      'help' => 'Aggregate results of votes cast on ' . $data['description'] . '.',
      'base' => 'votingapi_cache',
      'field' => 'entity_id',
      'relationship field' => $data['entity_id_column'],
      'handler' => 'votingapi_views_handler_relationship',
      'extra' => array(
        array(
          'field' => 'entity_type',
          'value' => $data['entity_type'],
          'numeric' => FALSE
        ),
      ),
    );
  }  
  
}